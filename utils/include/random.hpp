#ifndef BENCH_TIMER_HPP
#define BENCH_TIEMR_HPP

#include <algorithm>
#include <chrono>
#include <random>
#include <ranges>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include "utils.hpp"

namespace bench {

using ymd = std::chrono::year_month_day;

static std::random_device seed_random_device;
static std::mt19937 engine(seed_random_device());
static std::mt19937_64 engine64(seed_random_device());

inline static int rng() {
  return engine();
}

inline static int rng(int l, int r) {
  return engine() % (r - l) + l;
}

inline static int64_t rng64() {
  return engine64();
}

inline static int64_t rng64(int64_t l, int64_t r) {
  return engine64() % (r - l) + l;
}

template <typename T, int64_t L, int64_t R>
  requires std::is_integral_v<T> and (R - L > 0)
static std::vector<T> rng_vec(size_t size) {
  static_assert(!(std::is_unsigned_v<T> && L < 0),
                "L must no less than 0 when T is unsigned");
  constexpr int64_t N = R - L;
  auto res = std::vector<T>(size);
  std::ranges::generate(res, [&] { return L + engine64() % N; });
  return res;
}

template <typename T>
  requires std::is_integral_v<T>
static std::vector<T> rng_vec(size_t size, int64_t l, int64_t r) {
  const int64_t N = r - l;
  auto res = std::vector<T>(size);
  std::ranges::generate(res, [&] { return l + engine64() % N; });
  return res;
}

template <typename T, int64_t L>
  requires std::is_integral_v<T>
static std::vector<T> distinct_vec(size_t size) {
  static_assert(!(std::is_unsigned_v<T> && L < 0),
                "L must no less than 0 when T is unsigned");
  auto res = std::vector<T>(size);
  std::iota(res.begin(), res.end(), L);
  std::ranges::shuffle(res, engine64);
  return res;
}

template <typename T>
  requires std::is_integral_v<T>
static std::vector<T> distinct_vec(size_t size, int64_t l) {
  auto res = std::vector<T>(size);
  std::iota(res.begin(), res.end(), l);
  std::ranges::shuffle(res, engine64);
  return res;
}

///
static std::vector<std::string> rng_dates(size_t size,
                                          string_view l,
                                          string_view r) {
  std::vector<std::string> res(size);
  auto const lday = svtoymd(l);
  auto const rday = svtoymd(r);
  auto const diff = rday - lday;
  std::ranges::generate(res, [&] {
    return lday + std::chrono::days{engine64() % diff.operator unsigned int()};
  });
}

/// TODO: random graph
template <typename T>
concept Hashable = requires(T a) {
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

template <Hashable Node>
using Graph = std::unordered_map<Node, Node>;

using CPGraph = std::vector<std::vector<int>>;

template <Hashable Node>
static Graph<Node> rng_graph(size_t size) {
  return {};
}

template <Hashable Node>
static Graph<Node> rng_connected_graph(size_t size) {
  return {};
}

}  // namespace bench

#endif  // BENCH_RANDOM_HPP