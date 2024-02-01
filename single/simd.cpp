#include <algorithm>
#include <concepts>
#include <experimental/simd>
#include <iostream>
#include <ranges>
#include <vector>
namespace stdx = std::experimental;

// struct Pixel {
//   std::uint8_t r, g, b, a;
// };

// using Image = std::vector<Pixel>;

// void to_gray(Image& img) {
//   for (auto& [r, g, b, _] : img) {
//     auto const gray = (r * 11 + g * 16 + b * 5) / 32;
//     r = g = b = gray;
//   }
// }

// using Pixel = stdx::simd<std::uint8_t, 4>;
// using Image = std::vector<Pixel>;

constexpr std::uint32_t gray_coeff[4] = {5, 16, 11, 0};

int main() { return 0; }