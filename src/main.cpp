#include "jde/format.hpp"

int main(int argc, char const *argv[]) {
    using namespace jde::format::colors;
    std::println("{} {}\n"
                 "{} {}\n"
                 "{} {}\n"
                 "{} {}\n"
                 "{} {}\n"
                 "{} {}\n"
                 "{} {}\n"
                 "{} {}\n"
                 "{} {}\n"
                 "{} {}\n"
                 "{} {}\n"
                 "{} {}\n"
                 "{} {}\n"
                 "{} {}\n"
                 "{} {}\n"
                 "{} {}\n",
                 foreground::red % "red",
                 background::red % "red",
                 foreground::black % "black",
                 background::black % "black",
                 foreground::green % "green",
                 background::green % "green",
                 foreground::yellow % "yellow",
                 background::yellow % "yellow",
                 foreground::blue % "blue",
                 background::blue % "blue",
                 foreground::magenta % "magenta",
                 background::magenta % "magenta",
                 foreground::cyan % "cyan",
                 background::cyan % "cyan",
                 foreground::white % "white",
                 background::white % "white",
                 foreground::bright_black % "bright_black",
                 background::bright_black % "bright_black",
                 foreground::bright_red % "bright_red",
                 background::bright_red % "bright_red",
                 foreground::bright_green % "bright_green",
                 background::bright_green % "bright_green",
                 foreground::bright_yellow % "bright_yellow",
                 background::bright_yellow % "bright_yellow",
                 foreground::bright_blue % "bright_blue",
                 background::bright_blue % "bright_blue",
                 foreground::bright_magenta % "bright_magenta",
                 background::bright_magenta % "bright_magenta",
                 foreground::bright_cyan % "bright_cyan",
                 background::bright_cyan % "bright_cyan",
                 foreground::bright_white % "bright_white",
                 background::bright_white % "bright_white");

    std::println("{}\n"
                 "{}\n"
                 "{}\n"
                 "{}\n"
                 "{}\n"
                 "{}\n"
                 "{}\n"
                 "{}\n",
                 jde::format::bold % "bold",
                 jde::format::italic % "italic",
                 jde::format::faint % "faint",
                 jde::format::underline % "underline",
                 jde::format::slow_blink % "slow_blink",
                 jde::format::inverse % "inverse",
                 jde::format::hidden % "hidden",
                 jde::format::strikethrough % "strikethrough");

    return 0;
}
