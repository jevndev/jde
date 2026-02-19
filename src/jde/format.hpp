#include "jde/metaprogramming.hpp"

#include <print>

namespace jde {
namespace format {

namespace detail {

template <typename T, typename First, typename... Modifiers>
struct format_instance {
    using formatee_t = T;

    T m_t;
};

template <typename... Modifiers>
struct format_spec_builder;

template <jde::static_string s>
struct format_spec : format_spec_builder<format_spec<s>> {
    static constexpr auto parameter = s;
};

template <typename T>
struct is_format_spec : std::false_type {};

template <static_string s>
struct is_format_spec<format_spec<s>> : std::true_type {};

template <typename T>
inline constexpr bool is_format_spec_v = is_format_spec<std::remove_cvref_t<T>>::value;

template <typename... Modifiers>
struct format_spec_builder {
    template <static_string s>
    constexpr friend auto operator%(const format_spec_builder<Modifiers...> &in,
                                    const format_spec<s> &t) {
        return format_spec_builder<Modifiers..., format_spec<s>>{};
    }

    template <typename T>
        requires(!is_format_spec_v<T>)
    constexpr friend auto operator%(const format_spec_builder<Modifiers...> &in, T &&t) {
        return format_instance<std::decay_t<T>, Modifiers...>{std::forward<T>(t)};
    }
};

}; // namespace detail

namespace colors {
namespace foreground {

static constexpr auto red = detail::format_spec<"31">{};
static constexpr auto black = detail::format_spec<"30">{};
static constexpr auto green = detail::format_spec<"32">{};
static constexpr auto yellow = detail::format_spec<"33">{};
static constexpr auto blue = detail::format_spec<"34">{};
static constexpr auto magenta = detail::format_spec<"35">{};
static constexpr auto cyan = detail::format_spec<"36">{};
static constexpr auto white = detail::format_spec<"37">{};
static constexpr auto bright_black = detail::format_spec<"90">{};
static constexpr auto bright_red = detail::format_spec<"91">{};
static constexpr auto bright_green = detail::format_spec<"92">{};
static constexpr auto bright_yellow = detail::format_spec<"93">{};
static constexpr auto bright_blue = detail::format_spec<"94">{};
static constexpr auto bright_magenta = detail::format_spec<"95">{};
static constexpr auto bright_cyan = detail::format_spec<"96">{};
static constexpr auto bright_white = detail::format_spec<"97">{};

}; // namespace foreground

namespace background {

static constexpr auto red = detail::format_spec<"41">{};
static constexpr auto black = detail::format_spec<"40">{};
static constexpr auto green = detail::format_spec<"42">{};
static constexpr auto yellow = detail::format_spec<"43">{};
static constexpr auto blue = detail::format_spec<"44">{};
static constexpr auto magenta = detail::format_spec<"45">{};
static constexpr auto cyan = detail::format_spec<"46">{};
static constexpr auto white = detail::format_spec<"47">{};
static constexpr auto bright_black = detail::format_spec<"100">{};
static constexpr auto bright_red = detail::format_spec<"101">{};
static constexpr auto bright_green = detail::format_spec<"102">{};
static constexpr auto bright_yellow = detail::format_spec<"103">{};
static constexpr auto bright_blue = detail::format_spec<"104">{};
static constexpr auto bright_magenta = detail::format_spec<"105">{};
static constexpr auto bright_cyan = detail::format_spec<"106">{};
static constexpr auto bright_white = detail::format_spec<"107">{};

} // namespace background
} // namespace colors

static constexpr auto bold = detail::format_spec<"1">{};
static constexpr auto italic = detail::format_spec<"3">{};
static constexpr auto faint = detail::format_spec<"2">{};
static constexpr auto underline = detail::format_spec<"4">{};
static constexpr auto slow_blink = detail::format_spec<"5">{};
static constexpr auto inverse = detail::format_spec<"7">{};
static constexpr auto hidden = detail::format_spec<"8">{};
static constexpr auto strikethrough = detail::format_spec<"9">{};

static constexpr auto reset = "\033[0m";

} // namespace format
} // namespace jde

template <typename Instance>
    requires jde::is_specialization_of_v<Instance, jde::format::detail::format_instance>
struct std::formatter<Instance> {
    constexpr auto parse(std::format_parse_context &ctx) { return m_formatter.parse(ctx); }

    template <typename T, typename... Modifiers>
    auto format(const jde::format::detail::format_instance<T, Modifiers...> &modifier,
                std::format_context &ctx) const {

        std::format_to(ctx.out(), "\033[");
        (std::format_to(ctx.out(), ";{}", Modifiers::parameter), ...);
        std::format_to(ctx.out(), "m");
        m_formatter.format(modifier.m_t, ctx);
        return std::format_to(ctx.out(), "\033[0m");
    }

    std::formatter<typename Instance::formatee_t> m_formatter;
};
