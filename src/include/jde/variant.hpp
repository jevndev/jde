#pragma once

#include <format>
#include <type_traits>
#include <utility>
#include <variant>

namespace jde {

template <typename... T>
struct overloaded : public T... {
    using T::operator()...;
};

template <typename... T>
overloaded(T...) -> overloaded<T...>;

template <typename Fn>
    requires std::is_invocable_v<Fn>
struct otherwise {
    explicit otherwise(Fn &&fn) : m_fn{fn} {}

    auto operator()([[maybe_unused]] const auto &) { return m_fn(); }

private:
    Fn m_fn;
};

constexpr auto visit(auto &&variant, auto &&fns...) {
    return std::visit(overloaded(std::forward<decltype(fns)>(fns)),
                      std::forward<decltype(variant)>(variant));
}

} // namespace jde

template <typename... Ts>
struct std::formatter<std::variant<Ts...>> {
    constexpr auto parse(std::format_parse_context &ctx) {
        if ((ctx.begin() != ctx.end()) && (*ctx.begin() != '}')) {
            throw std::format_error("variant formatter does not support format specs yet");
        }
        return ctx.begin();
    }

    constexpr auto format(const std::variant<Ts...> &var, std::format_context &ctx) const {
        return std::visit(
            [&ctx, this](const auto &value) {
                using T = std::remove_cvref_t<decltype(value)>;
                return std::get<std::formatter<T>>(m_formatters).format(value, ctx);
            },
            var);
    }

    std::tuple<std::formatter<Ts>...> m_formatters;
};
