#include "jde/itertools.hpp"

#include <concepts>
#include <cstdint>
#include <cstring>
#include <ranges>

namespace jde {

namespace detail {

static inline uint32_t murmur3_scramble_32(const uint32_t in) {
    static constexpr auto c1 = 0xcc9e2d51;
    static constexpr auto c2 = 0x1b873593;
    return (((in * c1) << 15) | ((in * c1) >> 17)) * c2;
}

template <std::ranges::range R>
    requires std::convertible_to<std::ranges::range_value_t<R>, uint8_t>
static inline uint32_t murmur3_32(R &&r, const uint32_t seed = 0xDEADBEEF) {
    static constexpr auto r1 = 15;
    static constexpr auto r2 = 13;
    static constexpr auto m = 5;
    static constexpr auto n = 0xe6546b64;

    static constexpr auto h1 = 0x85ebca6b;
    static constexpr auto h2 = 0xc2b2ae35;

    size_t len = 0;
    uint32_t k;
    uint32_t hash = seed;

    for (auto chunk : r | jde::chunk(4)) {
        if (std::ranges::distance(chunk) < 4) {
            break;
        }
        memcpy(&k, &(*chunk.begin()), sizeof(uint32_t));

        hash ^= murmur3_scramble_32(k);
        hash = (hash << 13) | (hash >> 19);
        hash = (hash * m) + n;
        len += 4;
    }

    k = 0;
    for (auto it = r.begin(); it != r.end(); ++it) {
        k <<= 8;
        k |= *it;
    }
    len += std::distance(r.begin(), r.end()) - len;

    hash ^= murmur3_scramble_32(k) ^ len;
    hash ^= hash >> 16;
    hash *= h1;
    hash ^= hash >> 13;
    hash *= h2;
    hash ^= hash >> 16;

    return hash;
}
} // namespace detail
} // namespace jde
