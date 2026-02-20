#include "jde/format.hpp"
#include <cstdint>

consteval float resolve_component(const float k) {
    if (k < 1.0f) {
        return k;
    } else if (k < 3.0f) {
        return 1.0f;
    } else if (k < 4.0f) {
        return 4.0f - k;
    }
    return 0.0f;
}

consteval uint8_t hsv_to_rgb(const float h, const float s, const float v, const int n) {
    const auto theta = n + h / 60.0f;

    auto k = (theta)-6.0f * static_cast<int>((theta) / 6.0f);
    while (k < 0)
        k += 6.0f;

    return static_cast<uint8_t>(std::clamp(v - v * s * resolve_component(k), 0.0f, 1.0f) * 255.0f);
}

template <int Hue, int Value>
consteval auto get_foreground_rgb_color() {
    using namespace jde::format::colors;
    static constexpr auto h = Hue * 360.0f / 30.0f;
    static constexpr auto s = 1.0f;
    static constexpr auto v = Value / 30.0f;

    static constexpr auto r = hsv_to_rgb(h, s, v, 5);
    static constexpr auto g = hsv_to_rgb(h, s, v, 3);
    static constexpr auto b = hsv_to_rgb(h, s, v, 1);

    return foreground::rgb<r, g, b>;
}

template <int Hue, int Value>
consteval auto get_background_rgb_color() {
    using namespace jde::format::colors;
    static constexpr auto h = Hue * 360.0f / 30.0f;
    static constexpr auto s = 1.0f;
    static constexpr auto v = Value / 30.0f;

    static constexpr auto r = hsv_to_rgb(h, s, v, 5);
    static constexpr auto g = hsv_to_rgb(h, s, v, 3);
    static constexpr auto b = hsv_to_rgb(h, s, v, 1);

    return background::rgb<r, g, b>;
}

template <std::size_t TopRow, std::size_t BottomRow, std::size_t... Cols>
void print_row_pair(std::index_sequence<Cols...>) {
    (std::print("{}",
                get_foreground_rgb_color<Cols, TopRow>() %
                    get_background_rgb_color<Cols, BottomRow>() % "▀"),
     ...);
    std::println("");
}

template <std::size_t... RowPairs, std::size_t N>
void print_all_rows(std::index_sequence<RowPairs...>, std::integral_constant<std::size_t, N>) {
    (print_row_pair<RowPairs * 2, RowPairs * 2 + 1>(std::make_index_sequence<N>{}), ...);
}

template <std::size_t N, std::size_t M>
void print_gradient() {
    print_all_rows(std::make_index_sequence<M / 2>{}, std::integral_constant<std::size_t, N>{});
}

int main(int argc, char const *argv[]) {
    print_gradient<30, 30>();

    return 0;
}
