#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

static std::array<float, 257> entropy_table;

void setup_entropy_table() {
  for (int i = 1; i <= 256; ++i) {
    float p = static_cast<float>(i) / 256;
    entropy_table[i] = -p * std::log2(p);
  }
}

float calc_chunk_entropy(const uint8_t* chunk) {
  std::array<int, 256> hist{};
  for (size_t i = 0; i < 256; ++i) {
    ++hist[chunk[i]];
  }
  float entropy = 0;
  for (int count : hist) {
    if (count == 0) {
      continue;
    }
    float p = static_cast<float>(count) / 256;
    entropy -= p * std::log2(p);
  }
  return entropy;
}

float fast_calc_chunk_entropy(const uint8_t* chunk) {
  std::array<int, 256> hist{};
  for (size_t i = 0; i < 256; ++i) {
    ++hist[chunk[i]];
  }
  float entropy = 0;
  for (int count : hist) {
    if (count == 0) {
      continue;
    }
    entropy += entropy_table[count];
  }
  return entropy;
}

float fast2_calc_chunk_entropy(const uint8_t* chunk) {
  std::array<int, 256> hist{};
  for (size_t i = 0; i < 256; ++i) {
    ++hist[chunk[i]];
  }
  float entropy = 0;
  for (int count : hist) {
    entropy += entropy_table[count];
  }
  return entropy;
}

volatile double sink{};  // ensures a side effect

int main(int, const char* argv[]) {
  setup_entropy_table();

  std::vector<uint8_t> content;
  if (std::ifstream ifs{argv[0], std::ios::ate}; ifs.is_open()) {
    auto size = ifs.tellg();
    content.resize(size);
    ifs.seekg(0);
    ifs.read((char*)content.data(), content.size());
  } else {
    std::terminate();
  }

  auto benchmark = [&content](auto fun, auto rem) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto count = content.size() / 256;
    const auto start = std::chrono::steady_clock::now();
    for (auto i = 0; i != 1'000; ++i) {
      for (auto j = 0; j < count; ++j) {
        sink = fun(content.data() + (j * 256));
      }
    }
    const std::chrono::duration<double, std::milli> diff =
        std::chrono::steady_clock::now() - start;
    std::cout << "Time: " << std::fixed << std::setprecision(4) << diff.count()
              << "ms " << rem << std::endl;
  };

  benchmark(calc_chunk_entropy, "calc_entropy");
  benchmark(fast2_calc_chunk_entropy, "fast2_calc_entropy");
  benchmark(fast_calc_chunk_entropy, "fast_calc_entropy");

  return 0;
}