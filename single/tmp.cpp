#include <algorithm>
#include <execution>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <string>
#include <vector>
#include "../tbs/include/random.hpp"
#include "../tbs/include/timer.hpp"

std::mt19937 rng(std::random_device{}());
using tbs::Timer;

int main() {
  Timer timer;
  std::vector<int> a(1e6);
  std::ranges::generate(a, rng);
  std::cout << std::accumulate(a.begin(), a.end(), 0) << '\n';
  std::cout << timer << '\n';
  timer.reset();
  std::ranges::sort(a);
  std::cout << timer << '\n';
  timer.reset();
  std::cout << std::accumulate(a.begin(), a.end(), 0) << '\n';
  std::cout << timer << '\n';
  return 0;
}