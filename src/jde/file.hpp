#pragma once

#include <__ranges/range_adaptor.h>
#include <format>
#include <istream>
#include <iterator>
#include <ranges>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace jde {

namespace exceptions
{

class FileDoesntExist: public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

}

namespace detail
{

template <typename CharT = char>
class char_view
{
public:
    using It = std::istreambuf_iterator<CharT>;

    constexpr explicit char_view(const std::filesystem::path& path): m_is{path} {}
    [[nodiscard]] constexpr auto begin() {return It{m_is};};
    [[nodiscard]] constexpr auto end() {return It{};};
private:
    std::ifstream m_is;
};
}


template <typename CharT = char>
[[nodiscard]] constexpr auto view_chars(std::istream& is) noexcept {
    using It = std::istreambuf_iterator<CharT>;
    return std::ranges::subrange(It{is}, It{});
}

template <typename CharT = char>
[[nodiscard]] constexpr auto view_chars(const std::filesystem::path& path) {
    if (std::filesystem::exists(path))
    {
        return detail::char_view<CharT>{path};
    }
    throw exceptions::FileDoesntExist{path.string()};
}

}


template <>
struct std::formatter<std::filesystem::path>
{
    constexpr auto parse(std::format_parse_context& ctx) noexcept
    {
        return ctx.begin();
    }

    constexpr auto format(const std::filesystem::path& path, std::format_context& ctx) const noexcept
    {
        return std::format_to(ctx.out(), "{}", path.string());
    }
};
