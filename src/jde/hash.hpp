#include "jde/concepts.hpp"
#include "jde/itertools.hpp"
#include "jde/metaprogramming.hpp"

#include <concepts>
#include <cstdint>
#include <cstring>
#include <ranges>
#include <type_traits>

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

static inline uint64_t murmur3_scramble_64(const uint64_t in) {
    static constexpr auto c1 = 0xff51afd7ed558ccdULL;
    static constexpr auto c2 = 0xc4ceb9fe1a85ec53ULL;
    return (((in * c1) << 31) | ((in * c1) >> 33)) * c2;
}

template <std::ranges::range R>
    requires std::convertible_to<std::ranges::range_value_t<R>, uint8_t>
static inline uint64_t murmur3_64(R &&r, const uint64_t seed = 0xFEEDBABEDEADBEEFULL) {
    static constexpr auto r1 = 31;
    static constexpr auto r2 = 27;
    static constexpr auto m = 5;
    static constexpr auto n = 0x85ebca6b85ebca6bULL;

    static constexpr auto h1 = 0xff51afd7ed558ccdULL;
    static constexpr auto h2 = 0xc4ceb9fe1a85ec53ULL;

    size_t len = 0;
    uint64_t k;
    uint64_t hash = seed;

    for (auto chunk : r | jde::chunk(8)) {
        if (std::ranges::distance(chunk) < 8) {
            break;
        }
        memcpy(&k, &(*chunk.begin()), sizeof(uint64_t));

        hash ^= murmur3_scramble_64(k);
        hash = (hash << 27) | (hash >> 37);
        hash = (hash * m) + n;
        len += 8;
    }

    k = 0;
    for (auto it = r.begin(); it != r.end(); ++it) {
        k <<= 8;
        k |= *it;
    }
    len += std::distance(r.begin(), r.end()) - len;

    hash ^= murmur3_scramble_64(k) ^ len;
    hash ^= hash >> 33;
    hash *= h1;
    hash ^= hash >> 33;
    hash *= h2;
    hash ^= hash >> 33;

    return hash;
}

} // namespace detail

template <typename... Ts>
    requires all_satisfy<[](jde::hashable auto) {}, Ts...>
size_t combine_hashes(const Ts &...ts) {
    std::vector<uint8_t> hashes;
    hashes.reserve(sizeof...(ts) * sizeof(std::size_t));
    (
        [&](const auto t) {
            const size_t hash = std::hash<std::remove_cvref_t<decltype(t)>>{}(t);
            const auto *bytes = reinterpret_cast<const uint8_t *>(&hash);
            hashes.insert(hashes.end(), bytes, bytes + sizeof(hash));
        }(ts),
        ...);

    return detail::murmur3_64(hashes);
}

} // namespace jde
