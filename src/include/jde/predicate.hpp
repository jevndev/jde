#pragma once

#include "jde/metaprogramming.hpp"

#include <concepts>
#include <functional>

namespace jde {

namespace detail {

template <auto Projection, template <typename> typename Comparator, typename T>
concept projection_result_comparable =
    std::predicate<Comparator<projection_result_t<Projection, T>>,
                   const projection_result_t<Projection, T> &,
                   const projection_result_t<Projection, T> &>;

template <auto Projection, template <typename> typename Predicate>
struct ProjectingPredicate {
    template <typename T>
        requires projection_result_comparable<Projection, Predicate, T>
    [[gnu::always_inline]] [[nodiscard]] constexpr bool operator()(const T &lhs,
                                                                   const T &rhs) const noexcept {
        static constexpr auto pred = Predicate<projection_result_t<Projection, T>>{};

        return pred(std::invoke(Projection, lhs), std::invoke(Projection, rhs));
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
