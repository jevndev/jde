#pragma once

#include <algorithm>
#include <format>
#include <tuple> // IWYU pragma: keep
#include <type_traits>

namespace jde {

namespace detail {
template <typename Fn>
struct object_type;

template <typename T, typename R, typename... Args>
struct object_type<R (T::*)(Args...)> {
    using type = T;
};

template <typename T, typename R, typename... Args>
struct object_type<R (T::*)(Args...) const> {
    using type = T;
};

template <typename Fn>
struct return_type;

template <typename T, typename R, typename... Args>
struct return_type<R (T::*)(Args...)> {
    using type = R;
};

template <typename T, typename R, typename... Args>
struct return_type<R (T::*)(Args...) const> {
    using type = R;
};

template <typename T, template <typename...> typename Primary>
struct is_specialization_of : std::false_type {};

template <template <typename...> typename Primary, typename... Args>
struct is_specialization_of<Primary<Args...>, Primary> : std::true_type {};

} // namespace detail

namespace member_function {
template <auto Method>
using return_type = typename detail::return_type<decltype(Method)>::Type;

template <auto Method>
using object_type = typename detail::object_type<decltype(Method)>::Type;

} // namespace member_function

template <typename T, template <typename...> typename Primary>
inline constexpr bool is_specialization_of_v = detail::is_specialization_of<T, Primary>::value;

template <std::size_t N>
struct static_string {
    char data[N]{};

    constexpr static_string(const char (&str)[N]) { std::copy_n(str, N, data); }
    constexpr operator const char *() const { return data; }

    template <std::size_t M>
    friend constexpr static_string<N + M - 1> operator+(const static_string<N> &lhs,
                                                        const static_string<M> &rhs) noexcept {
        char out[N + M - 1];
        std::copy_n(lhs.data, N - 1, out);
        std::copy_n(rhs.data, M, out + N - 1);
        return out;
    }

    template <std::size_t M>
    friend constexpr auto operator+(const static_string<N> &lhs, const char (&rhs)[M]) noexcept {
        return lhs + static_string<M>{rhs};
    }
};

template <uint8_t V>
[[nodiscard]] constexpr auto to_static_string() {
    char result[] = {'0' + ((V / 100) % 10), '0' + ((V / 10) % 10), '0' + (V % 10), '\0'};
    return static_string{result};
}

template <auto Constraint, typename... Ts>
concept all_satisfy = requires(Ts... ts) { (Constraint(ts), ...); };

template <jde::static_string S>
[[nodiscard]] constexpr auto operator""_ss() {
    return S;
}

} // namespace jde

template <size_t N>
struct std::formatter<jde::static_string<N>> {
    constexpr auto parse(std::format_parse_context &ctx) { return m_formatter.parse(ctx); }

    auto format(const jde::static_string<N> &str, std::format_context &ctx) const {
        return m_formatter.format(str.data, ctx);
    }

    std::formatter<const char *> m_formatter;
};
