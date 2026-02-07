#pragma once

#include <concepts>
#include <ostream>
#include <type_traits>
#include <utility>

#include "metaprogramming.hpp"

namespace jde {

template <typename T>
concept Pointer = requires(T t) { *t; } && std::is_convertible_v<T, void *>;

template <typename T, template <typename...> typename U>
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
concept Tuple = EmptyTuple<T> || NonEmptyTuple<T>;

template <typename T>
concept Hashable = requires(T t) {
    { std::hash<T>{}(t) } -> std::convertible_to<size_t>;
};

template <typename T>
concept OStreamable = requires(T t) {
    { std::declval<std::ostream &>() << t } -> std::convertible_to<std::ostream &>;
};

template <typename T>
concept IStreamable = requires(T t) {
    { std::declval<std::istream &>() >> t } -> std::convertible_to<std::istream &>;
};

} // namespace jde
