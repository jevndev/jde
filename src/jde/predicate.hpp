#pragma once

#include "jde/metaprogramming.hpp"
#include <concepts>

namespace jde {

namespace detail {

template <auto Projection, typename Comparator>
concept projection_result_comparable =
    std::predicate<Comparator, const member_function::return_type<Projection> &,
                   const member_function::return_type<Projection> &>;

template <auto Projection, template <typename> typename Predicate>
    requires projection_result_comparable<Projection,
                                          Predicate<member_function::return_type<Projection>>>
struct ProjectingPredicate {
    [[gnu::always_inline]] [[nodiscard]] constexpr bool
    operator()(const member_function::object_type<Projection> &lhs,
               const member_function::object_type<Projection> &rhs) const noexcept {
        static constexpr auto pred = Predicate<member_function::return_type<Projection>>{};
        return pred((lhs.*Projection)(), (rhs.*Projection)());
    }
};

} // namespace detail

template <typename T>
struct less {
    [[gnu::always_inline]] [[nodiscard]] constexpr bool operator()(const T &lhs,
                                                                   const T &rhs) const noexcept {
        return lhs < rhs;
    }
};

template <auto Projection>
struct less_by : public detail::ProjectingPredicate<Projection, less> {};

template <typename T>
struct less_equal {
    [[gnu::always_inline]] [[nodiscard]] constexpr bool operator()(const T &lhs,
                                                                   const T &rhs) const noexcept {
        return lhs <= rhs;
    }
};

template <auto Projection>
struct less_equal_by : public detail::ProjectingPredicate<Projection, less_equal> {};

template <typename T>
struct greater {
    [[gnu::always_inline]] [[nodiscard]] constexpr bool operator()(const T &lhs,
                                                                   const T &rhs) const noexcept {
        return lhs > rhs;
    }
};

template <auto Projection>
struct greater_by : public detail::ProjectingPredicate<Projection, greater> {};

template <typename T>
struct greater_equal {
    [[gnu::always_inline]] [[nodiscard]] constexpr bool operator()(const T &lhs,
                                                                   const T &rhs) const noexcept {
        return lhs >= rhs;
    }
};

template <auto Projection>
struct greater_equal_by : public detail::ProjectingPredicate<Projection, greater_equal> {};

template <typename T>
struct equal_to {
    [[gnu::always_inline]] [[nodiscard]] constexpr bool operator()(const T &lhs,
                                                                   const T &rhs) const noexcept {
        return lhs < rhs;
    }
};

template <auto Projection>
struct equal_to_by : public detail::ProjectingPredicate<Projection, equal_to> {};

} // namespace jde
