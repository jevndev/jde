#include "jde/file.hpp"
#include <filesystem>

int main()
{
    const auto p = __FILE__;
    for (const auto c: jde::view_chars(p))
    {
        std::println("_{}_", c);
    }
}
