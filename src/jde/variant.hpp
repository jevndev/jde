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

template <typename Fn>
requires std::is_invocable_v<Fn>
struct Default
{
    explicit Default(Fn&& fn): m_fn{fn} {}

    auto operator()([[maybe_unused]] const auto&){
        return m_fn();
    }

private:
    Fn m_fn;
};

constexpr auto visit(auto &&variant, auto &&fns...) {
    return std::visit(overloaded(std::forward<decltype(fns)>(fns)),
                      std::forward<decltype(variant)>(variant));
}

} // namespace jde
