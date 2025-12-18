#pragma once

#include <ranges>

namespace jde
{

template <typename V>
  requires std::ranges::view<V>
struct cumulative_sum_view : public std::ranges::view_interface<cumulative_sum_view<V>>
{
public:
  using T = std::ranges::range_value_t<V>;
  using UnderlyingIteratorT = std::ranges::iterator_t<V>;

  class Iter
  {
  public:
    using difference_type = UnderlyingIteratorT::difference_type;
    using value_type = T;

    constexpr explicit Iter() noexcept = default;

    constexpr explicit Iter(const UnderlyingIteratorT& it, const UnderlyingIteratorT& end) noexcept
        : m_curr { it }, m_end { end }
    {
      if (m_curr != m_end)
      {
        m_accumulator = *m_curr;
      }
    }

    constexpr Iter(const Iter&) = default;
    constexpr Iter& operator=(const Iter&) = default;
    constexpr Iter& operator=(Iter&&) = default;
    constexpr Iter(Iter&& other) noexcept = default;
    constexpr ~Iter() = default;

    constexpr Iter& operator++() {
      if (m_curr != m_end)
      {
        m_curr++;
        if (m_curr != m_end) m_accumulator += *m_curr;
      }
      return *this;
    }

    constexpr Iter operator++(int) {
      if (m_curr != m_end)
      {
        auto tmp = *this;
        ++(*this);
        return tmp;
      } else {
        return *this;
      }
    }

    [[nodiscard]] constexpr bool operator==(const Iter& rhs) const {
      return m_curr == rhs.m_curr && m_end == rhs.m_end;
    }

    [[nodiscard]] constexpr bool operator==([[maybe_unused]] const std::default_sentinel_t sentinel) const {
      return m_curr == m_end;
    }

    [[nodiscard]] constexpr T operator*() const {return m_accumulator;}

  private:
    UnderlyingIteratorT m_curr;
    UnderlyingIteratorT m_end;
    T m_accumulator;
  };

  constexpr explicit cumulative_sum_view(V&& view) noexcept
      : m_range { std::move(view) }
  {
  }

  template <typename Self>
  [[nodiscard]] constexpr auto begin(this Self&& self) {return Iter{std::ranges::begin(std::forward<Self>(self).m_range), std::ranges::end(std::forward<Self>(self).m_range)};}
  [[nodiscard]] constexpr auto end() const {return std::default_sentinel;}

private:
  V m_range;
};

struct cumulative_sum_t : public std::ranges::range_adaptor_closure<cumulative_sum_t>
{
  template <typename R>
    requires std::ranges::viewable_range<R>
  constexpr auto operator()(const R& r) const
  {
    return cumulative_sum_view { std::views::all(r) };
  }
};

static constexpr inline auto cumulative_sum = cumulative_sum_t{};

}
