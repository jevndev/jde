#include "jde/all.hpp" // IWYU pragma: keep

#include <gtest/gtest.h>

#include <iostream>
#include <vector>

struct OStreamable_implementation {
    friend std::ostream &operator<<(std::ostream &os, const OStreamable_implementation &) {
        return os;
    }
};

struct IStreamable_implementation {
    friend std::istream &operator>>(std::istream &is, IStreamable_implementation &) { return is; }
};

struct Hashable_implementation {};

namespace std {
template <>
struct hash<Hashable_implementation> {
    size_t operator()(const Hashable_implementation &) { return 0; }
};
}; // namespace std

static_assert(jde::output_streamable<OStreamable_implementation>);
static_assert(jde::input_streamable<IStreamable_implementation>);
static_assert(jde::hashable<Hashable_implementation>);
static_assert(jde::is_specialization_of_v<std::vector<int>, std::vector>);
static_assert(std::is_same_v<decltype(jde::first(std::tuple<int, float>{})), int>);
static_assert(std::is_same_v<decltype(jde::second(std::tuple<int, float>{})), float>);

TEST(jde, static_tests_pass) { SUCCEED(); }

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
