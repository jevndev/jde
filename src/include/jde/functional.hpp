#pragma once

#include <cstddef>
#include <utility>

namespace jde {

// allows for passing as NTTP w/ type deduction, particularly useful with `predicate.hpp`
template <size_t N>
static constexpr auto get = [](auto &&t) { return std::get<N>(std::forward<decltype(t)>(t)); };

static constexpr auto first = get<0>;
static constexpr auto second = get<1>;

}; // namespace jde
