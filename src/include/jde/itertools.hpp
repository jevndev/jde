#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <ranges>
#include <vector>

#include "jde/concepts.hpp"

namespace jde {

namespace detail {

template <typename V, typename AccumulatorType>
class cumulative_sum_iter {
public:
    using T = std::ranges::range_value_t<V>;
    using underlying_iterator_t = std::ranges::iterator_t<V>;
    using difference_type = std::iter_difference_t<underlying_iterator_t>;
    using value_type = T;

    constexpr explicit cumulative_sum_iter(underlying_iterator_t it, underlying_iterator_t end,
                                           AccumulatorType init) noexcept
        : m_curr{it}, m_end{end}, m_accumulator{std::move(init)} {
        if (m_curr != m_end) {
            m_accumulator += *m_curr;
        }
    }

    constexpr cumulative_sum_iter(const cumulative_sum_iter &) = default;
    constexpr cumulative_sum_iter &operator=(const cumulative_sum_iter &) = default;
    constexpr cumulative_sum_iter &operator=(cumulative_sum_iter &&) = default;
    constexpr cumulative_sum_iter(cumulative_sum_iter &&other) noexcept = default;
    constexpr ~cumulative_sum_iter() = default;

    constexpr cumulative_sum_iter &operator++() {
        if (m_curr != m_end) {
            m_curr++;
            if (m_curr != m_end)
                m_accumulator += *m_curr;
        }
        return *this;
    }

    constexpr cumulative_sum_iter operator++(int) {
        if (m_curr != m_end) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        } else {
            return *this;
        }
    }

    [[nodiscard]] constexpr bool operator==(const cumulative_sum_iter &rhs) const {
        return m_curr == rhs.m_curr && m_end == rhs.m_end;
    }

    [[nodiscard]] constexpr bool
    operator==([[maybe_unused]] const std::default_sentinel_t sentinel) const {
        return m_curr == m_end;
    }

    [[nodiscard]] constexpr T operator*() const { return m_accumulator; }

private:
    underlying_iterator_t m_curr;
    underlying_iterator_t m_end;
    T m_accumulator;
};

template <typename V, typename AccumulatorType>
class cumulative_sum_view
    : public std::ranges::view_interface<cumulative_sum_view<V, AccumulatorType>> {
public:
    using T = std::ranges::range_value_t<V>;

    constexpr explicit cumulative_sum_view(V view, AccumulatorType init) noexcept
        : m_range{std::move(view)}, m_init{std::move(init)} {}

    [[nodiscard]] constexpr auto begin() {
        return cumulative_sum_iter<V, AccumulatorType>{std::ranges::begin(m_range),
                                                       std::ranges::end(m_range),
                                                       AccumulatorType{m_init}};
    }

    [[nodiscard]] constexpr auto begin() const {
        return cumulative_sum_iter<const V, AccumulatorType>{std::ranges::begin(m_range),
                                                             std::ranges::end(m_range),
                                                             AccumulatorType{m_init}};
    }

    [[nodiscard]] constexpr auto end() const { return std::default_sentinel; }

private:
    V m_range;
    AccumulatorType m_init;
};

} // namespace detail

template <typename AccumulatorType = void>
class cumulative_sum : public std::ranges::range_adaptor_closure<cumulative_sum<AccumulatorType>> {
public:
    constexpr explicit cumulative_sum() noexcept : m_init{} {}

    constexpr explicit cumulative_sum(const AccumulatorType &init) noexcept : m_init{init} {}

    template <typename R>
        requires std::ranges::viewable_range<R> &&
                 std::convertible_to<std::ranges::range_value_t<R>, AccumulatorType>
    constexpr auto operator()(const R &r) const {
        return detail::cumulative_sum_view{std::views::all(r), AccumulatorType{m_init}};
    }

private:
    AccumulatorType m_init;
};

template <>
class cumulative_sum<void> : public std::ranges::range_adaptor_closure<cumulative_sum<void>> {
public:
    constexpr explicit cumulative_sum() noexcept = default;

    template <std::ranges::viewable_range R>
    constexpr auto operator()(const R &r) const {
        return detail::cumulative_sum_view{std::views::all(r), std::ranges::range_value_t<R>{}};
    }
};

template <typename T>
cumulative_sum(T) -> cumulative_sum<T>;

template <std::ranges::viewable_range R>
constexpr auto sum(R &&r) {
    using T = std::ranges::range_value_t<R>;
    return std::ranges::fold_left(std::move(r), T{}, std::plus<T>{});
}

template <std::ranges::viewable_range R>
constexpr auto stddev(R &&r) {
    using T = std::ranges::range_value_t<R>;

    std::vector<T> data;
    for (const auto &val : r) {
        data.push_back(val);
    }

    const auto n = data.size();
    if (n == 0)
        return T{};

    if constexpr (jde::is_specialization_of_v<T, std::chrono::duration>) {
        using Rep = typename T::rep;

        auto mean_val = sum(data) / static_cast<Rep>(n);

        Rep sum_squared_diff = 0;
        for (const auto &val : data) {
            auto diff = (val - mean_val).count();
            sum_squared_diff += diff * diff;
        }

        return T{static_cast<Rep>(std::sqrt(static_cast<double>(sum_squared_diff) / n))};
    } else {
        auto mean_val = sum(data) / static_cast<T>(n);

        T sum_squared_diff{};
        for (const auto &val : data) {
            auto diff = val - mean_val;
            sum_squared_diff += diff * diff;
        }

        return std::sqrt(sum_squared_diff / static_cast<T>(n));
    }
}

template <typename... Pipelines>
class split_pipeline : public std::ranges::range_adaptor_closure<split_pipeline<Pipelines...>> {
public:
    constexpr split_pipeline(Pipelines &&...pipelines)
        : m_pipelines{std::forward<Pipelines>(pipelines)...} {};

    template <std::ranges::viewable_range R>
        requires jde::tuple<std::ranges::range_value_t<R>>
    constexpr auto operator()(R &&r) const {
        return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
            auto get_nth = [&]<std::size_t I> {
                return std::forward<R>(r) | std::views::transform(std::get<I>) |
                       std::get<I>(m_pipelines);
            };
            return std::views::zip(get_nth.template operator()<Is>()...);
        }(std::make_index_sequence<sizeof...(Pipelines)>{});
    }

private:
    std::tuple<Pipelines...> m_pipelines;
};

template <typename... Pipelines>
split_pipeline(Pipelines &&...) -> split_pipeline<Pipelines...>;

namespace detail {

template <typename V>
class chunk_iter {
public:
    using underlying_iterator_t = std::ranges::iterator_t<V>;
    using difference_type = std::iter_difference_t<underlying_iterator_t>;

    constexpr explicit chunk_iter(underlying_iterator_t it, underlying_iterator_t end,
                                  std::size_t chunk_size) noexcept
        : m_curr{it}, m_end{end}, m_chunk_size{chunk_size} {}

    constexpr chunk_iter(const chunk_iter &) = default;
    constexpr chunk_iter &operator=(const chunk_iter &) = default;
    constexpr chunk_iter &operator=(chunk_iter &&) = default;
    constexpr chunk_iter(chunk_iter &&other) noexcept = default;
    constexpr ~chunk_iter() = default;

    constexpr chunk_iter &operator++() {
        for (std::size_t i = 0; i < m_chunk_size && m_curr != m_end; ++i) {
            ++m_curr;
        }
        return *this;
    }

    constexpr chunk_iter operator++(int) {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    [[nodiscard]] constexpr bool operator==(const chunk_iter &rhs) const {
        return m_curr == rhs.m_curr;
    }

    [[nodiscard]] constexpr bool operator==(const std::default_sentinel_t) const {
        return m_curr == m_end;
    }

    [[nodiscard]] constexpr auto operator*() const {
        return std::ranges::subrange(m_curr, std::ranges::next(m_curr, m_chunk_size, m_end));
    }

private:
    underlying_iterator_t m_curr;
    underlying_iterator_t m_end;
    std::size_t m_chunk_size;
};

template <typename V>
class chunk_view : public std::ranges::view_interface<chunk_view<V>> {
public:
    constexpr explicit chunk_view(V view, std::size_t chunk_size) noexcept
        : m_range{std::move(view)}, m_chunk_size{chunk_size} {}

    [[nodiscard]] constexpr auto begin() {
        return chunk_iter<V>{std::ranges::begin(m_range), std::ranges::end(m_range), m_chunk_size};
    }

    [[nodiscard]] constexpr auto end() const { return std::default_sentinel; }

private:
    V m_range;
    std::size_t m_chunk_size;
};

} // namespace detail

class chunk : public std::ranges::range_adaptor_closure<chunk> {
public:
    constexpr explicit chunk(std::size_t chunk_size) noexcept : m_chunk_size{chunk_size} {}

    template <std::ranges::viewable_range R>
    constexpr auto operator()(R &&r) const {
        return detail::chunk_view{std::views::all(r), m_chunk_size};
    }

private:
    std::size_t m_chunk_size;
};

} // namespace jde
