#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iostream>

#include <dmath/dmath.hpp>

template<dmath::numeric T, std::size_t Rows, std::size_t Columns>
void applyBayerDither(dmath::matrix<T, Rows, Columns>& img) {
    // 4x4 Bayer matrix normalized to [0.0, 1.0)
    constexpr dmath::matrix<float, 4, 4> bayer4x4 = {{
        {  0.0f / 16.0f,  8.0f / 16.0f,  2.0f / 16.0f, 10.0f / 16.0f },
        { 12.0f / 16.0f,  4.0f / 16.0f, 14.0f / 16.0f,  6.0f / 16.0f },
        {  3.0f / 16.0f, 11.0f / 16.0f,  1.0f / 16.0f,  9.0f / 16.0f },
        { 15.0f / 16.0f,  7.0f / 16.0f, 13.0f / 16.0f,  5.0f / 16.0f }
    }};

    for (std::size_t r = 1; r <= Rows; ++r) {
        for (std::size_t c = 1; c <= Columns; ++c) {
            // Map 1-based (r, c) into 1-based [1..4] Bayer indices
            std::size_t br = ((r - 1) % 4) + 1;
            std::size_t bc = ((c - 1) % 4) + 1;
            float threshold = bayer4x4(br, bc);

            if constexpr (std::floating_point<T>) {
                img(r, c) = (img(r, c) > static_cast<T>(threshold)) ? T{1} : T{0};
            } else {
                T scaledThreshold = static_cast<T>(threshold * 255.0f);
                img(r, c) = (img(r, c) > scaledThreshold) ? T{255} : T{0};
            }
        }
    }
}

int main() {
    dmath::matrix<float, 8, 8> image{};
    for (std::size_t r = 1; r <= 8; ++r) {
        for (std::size_t c = 1; c <= 8; ++c) {
            image(r, c) = static_cast<float>(c - 1) / 7.0f;
        }
    }

    applyBayerDither(image);

    // Print the dithered result using ASCII characters
    for (std::size_t r = 1; r <= 8; ++r) {
        for (std::size_t c = 1; c <= 8; ++c) {
            std::cout << (image(r, c) > 0.5f ? '#' : '.') << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}

