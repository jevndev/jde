#pragma once

#include <tuple> // IWYU pragma: keep
#include <type_traits>

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

template<typename T, template<typename...> typename Primary>
struct is_specialization_of : std::false_type {};

template<template<typename...> typename Primary, typename... Args>
struct is_specialization_of<Primary<Args...>, Primary> : std::true_type {};

} // namespace detail

namespace MemberFunction {
template <auto Method>
using ReturnType = typename detail::GetReturnType<decltype(Method)>::Type;

template <auto Method>
using ObjectType = typename detail::GetObjType<decltype(Method)>::Type;

} // namespace MemberFunction

template<typename T, template<typename...> typename Primary>
inline constexpr bool is_specialization_of_v = detail::is_specialization_of<T, Primary>::value;

} // namespace jde
