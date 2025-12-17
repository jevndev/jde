#pragma once

#include <utility>
#include <variant>

namespace jde {

template <typename... T>
struct overloaded : public T... {
    using T::operator()...;
};

template <typename... T>
overloaded(T...) -> overloaded<T...>;

constexpr auto visit(auto &&variant, auto &&fns...) {
    return std::visit(overloaded(std::forward<decltype(fns)>(fns)),
                      std::forward<decltype(variant)>(variant));
}

} // namespace jde
