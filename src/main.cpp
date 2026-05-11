#include "jde/file.hpp"

#include <filesystem>

int main()
{
    std::filesystem::path p{__FILE__};

    std::println("{}", p);
}
