#pragma once

#include <tuple> // IWYU pragma: keep

namespace jde {

namespace detail {
template <typename Fn>
struct GetObjType;

template <typename T, typename R, typename... Args>
struct GetObjType<R (T::*)(Args...)> {
    using Type = T;
};

template <typename T, typename R, typename... Args>
struct GetObjType<R (T::*)(Args...) const> {
    using Type = T;
};

template <typename Fn>
struct GetReturnType;

template <typename T, typename R, typename... Args>
struct GetReturnType<R (T::*)(Args...)> {
    using Type = R;
};

template <typename T, typename R, typename... Args>
struct GetReturnType<R (T::*)(Args...) const> {
    using Type = R;
};

} // namespace detail

namespace MemberFunction {
template <auto Method>
using ReturnType = typename detail::GetReturnType<decltype(Method)>::Type;

template <auto Method>
using ObjectType = typename detail::GetObjType<decltype(Method)>::Type;

} // namespace MemberFunction

template <typename T>
concept is_empty_tuple_like = (std::tuple_size_v<T> == 0);

template <typename T>
concept is_nonempty_tuple_like = requires(T t) {
    std::tuple_element_t<0, T>{};
    std::get<0>(t);
    std::tuple_size_v<T>;
};

template <typename T>
concept is_tuple_like = is_empty_tuple_like<T> || is_nonempty_tuple_like<T>;

} // namespace jde
