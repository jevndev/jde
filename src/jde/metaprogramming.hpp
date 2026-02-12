#pragma once

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

} // namespace jde
