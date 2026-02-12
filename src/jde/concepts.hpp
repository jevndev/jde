#pragma once

#include <concepts>
#include <ostream>
#include <type_traits>
#include <utility>

#include "metaprogramming.hpp"

namespace jde {

template <typename T>
concept pointer = requires(T t) { *t; } && std::is_convertible_v<T, void *>;

template <typename T, template <typename...> typename U>
concept specialization_of = detail::is_specialization_of<T, U>::value;

template <typename T>
concept empty_tuple = (std::tuple_size_v<T> == 0);

template <typename T>
concept non_empty_tuple = requires(T t) {
    std::tuple_element_t<0, T>{};
    std::get<0>(t);
    std::tuple_size_v<T>;
};

template <typename T>
concept tuple = empty_tuple<T> || non_empty_tuple<T>;

template <typename T>
concept hashable = requires(T t) {
    { std::declval<std::hash<T>>()(t) } -> std::convertible_to<size_t>;
};

template <typename T>
concept output_streamable = requires(T t) {
    { std::declval<std::ostream &>() << t } -> std::convertible_to<std::ostream &>;
};

template <typename T>
concept input_streamable = requires(T t) {
    { std::declval<std::istream &>() >> t } -> std::convertible_to<std::istream &>;
};

} // namespace jde
