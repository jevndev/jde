#pragma once

#include "jde/metaprogramming.hpp"
#include <concepts>

namespace jde {

namespace detail {

template <auto Projection, typename Comparator>
concept ProjectionResultComparable =
    std::predicate<Comparator, const MemberFunction::ReturnType<Projection> &,
                   const MemberFunction::ReturnType<Projection> &>;

template <auto Projection, template <typename> typename Predicate>
    requires ProjectionResultComparable<
        Projection, Predicate<MemberFunction::ReturnType<Projection>>>
struct ProjectingPredicate {
    [[gnu::always_inline]] [[nodiscard]] constexpr bool operator()(
        const MemberFunction::ObjectType<Projection> &lhs,
        const MemberFunction::ObjectType<Projection> &rhs) const noexcept {
        static constexpr auto pred =
            Predicate<MemberFunction::ReturnType<Projection>>{};
        return pred((lhs.*Projection)(), (rhs.*Projection)());
    }
};

} // namespace detail

template <typename T>
struct less {
    [[gnu::always_inline]] [[nodiscard]] constexpr bool
    operator()(const T &lhs, const T &rhs) const noexcept {
        return lhs < rhs;
    }
};

template <auto Projection>
struct less_by : public detail::ProjectingPredicate<Projection, less> {};

template <typename T>
struct less_equal {
    [[gnu::always_inline]] [[nodiscard]] constexpr bool
    operator()(const T &lhs, const T &rhs) const noexcept {
        return lhs <= rhs;
    }
};

template <auto Projection>
struct less_equal_by
    : public detail::ProjectingPredicate<Projection, less_equal> {};

template <typename T>
struct greater {
    [[gnu::always_inline]] [[nodiscard]] constexpr bool
    operator()(const T &lhs, const T &rhs) const noexcept {
        return lhs > rhs;
    }
};

template <auto Projection>
struct greater_by : public detail::ProjectingPredicate<Projection, greater> {};

template <typename T>
struct greater_equal {
    [[gnu::always_inline]] [[nodiscard]] constexpr bool
    operator()(const T &lhs, const T &rhs) const noexcept {
        return lhs >= rhs;
    }
};

template <auto Projection>
struct greater_equal_by
    : public detail::ProjectingPredicate<Projection, greater_equal> {};

template <typename T>
struct equal_to {
    [[gnu::always_inline]] [[nodiscard]] constexpr bool
    operator()(const T &lhs, const T &rhs) const noexcept {
        return lhs < rhs;
    }
};

template <auto Projection>
struct equal_to_by : public detail::ProjectingPredicate<Projection, equal_to> {
};

} // namespace jde
