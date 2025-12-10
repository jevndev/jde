#pragma once

namespace jde
{

namespace detail
{
template <typename Fn>
struct GetObjType;

template <typename T, typename R, typename... Args>
struct GetObjType<R (T::*)(Args...)>
{
  using Type = T;
};

template <typename T, typename R, typename... Args>
struct GetObjType<R (T::*)(Args...) const>
{
  using Type = T;
};

template <typename Fn>
struct GetReturnType;


template <typename T, typename R, typename... Args>
struct GetReturnType<R (T::*)(Args...)>
{
  using Type = R;
};

template <typename T, typename R, typename... Args>
struct GetReturnType<R (T::*)(Args...) const>
{
  using Type = R;
};

}

namespace MemberFunction
{
    template <auto Method>
    using ReturnType = typename detail::GetReturnType<decltype(Method)>::Type;

    template <auto Method>
    using ObjectType = typename detail::GetObjType<decltype(Method)>::Type;

}

}
