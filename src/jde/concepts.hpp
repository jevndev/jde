#pragma once

#include <type_traits>

#include "metaprogramming.hpp"

namespace jde {

template <typename T>
concept Pointer = requires(T t) { *t; } && std::is_convertible_v<T, void *>;

template<typename T, template<typename...> typename U>
concept SpecializationOf = detail::is_specialization_of<T, U>::value;


template <typename T>
concept EmptyTuple = (std::tuple_size_v<T> == 0);

template <typename T>
concept NonEmptyTuple = requires(T t) {
    std::tuple_element_t<0, T>{};
    std::get<0>(t);
    std::tuple_size_v<T>;
};

template <typename T>
concept is_tuple_like = EmptyTuple<T> || NonEmptyTuple<T>;

} // namespace jde
