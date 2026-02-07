#pragma once

#include <type_traits>

#include "metaprogramming.hpp"

namespace jde {

template <typename T>
concept Pointer = requires(T t) { *t; } && std::is_convertible_v<T, void *>;

template<typename T, template<typename...> typename U>
concept SpecializationOf = detail::is_specialization_of<T, U>::value;

} // namespace jde
