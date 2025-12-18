#pragma once

#include <type_traits>
namespace jde {

template <typename T>
concept Pointer = requires(T t) { *t; } && std::is_convertible_v<T, void *>;

} // namespace jde
