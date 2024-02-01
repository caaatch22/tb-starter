#ifndef TBS_RANDOM_HPP
#define TBS_RAMDOM_HPP

#include <algorithm>
#include <random>
#include <ranges>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "utils.hpp"

namespace tbs {

static std::random_device seed_random_device;
static std::mt19937 engine(seed_random_device());
static std::mt19937_64 engine64(seed_random_device());

inline static int rng() { return engine(); }

inline static int rng(int low, int high) {
  return engine() % (high - low) + low;
}

inline static int64_t rng64() { return engine64(); }

inline static int64_t rng64(int64_t low, int64_t high) {
  return engine() % (high - low) + low;
}

inline static double rng_double() {
  return static_cast<double>(engine()) / engine.max();
}

template <std::integral T>
static std::vector<T> rng_vec(size_t size, int64_t low, int64_t high) {
  const int64_t N = high - low;
  auto res = std::vector<T>(size);
  std::ranges::generate(res, [&] { return low + engine64() % N; });
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
static std::vector<std::string> rng_dates(size_t size, std::string_view l,
                                          std::string_view r) {
  std::vector<std::string> res(size);
  auto const lymd = svtoymd(l);
  auto const rymd = svtoymd(r);
  auto const diff = (date::sys_days{rymd} - date::sys_days{lymd}).count();
  std::ranges::generate(res, [&] {
    auto const ymd = YMD{date::sys_days(lymd) + date::days{engine64() % diff}};
    return ymdtostr(ymd);
  });
  return res;
}

static std::string rng_string(size_t len) {
  std::string s(len, ' ');
  std::ranges::generate(s, [] { return 'a' + rng() % 26; });
  return s;
}

static std::vector<std::string> rng_strings(size_t size, size_t len) {
  std::vector<std::string> res(size);
  for (auto &s : res) {
    s = rng_string(len);
  }
  return res;
}

template <size_t N>
struct StringLiteral {
  constexpr StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); }
  char value[N];
};

// static std::vector<std::string> dates_between(std::string_view l,
//                                               std::string_view r) {
//   auto const lymd = svtoymd(l);
//   auto const rymd = svtoymd(r);
//   auto const diff = (date::sys_days{rymd} - date::sys_days{lymd}).count();
//   std::vector<std::string> res(diff);
//   std::ranges::generate(res, [&] {
//     auto const ymd = YMD{date::sys_days(lymd) + date::days{engine64() %
//     diff}}; return ymdtostr(ymd);
//   });
//   return res;
// }

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

}  // namespace tbs

#endif  // TBS_RANDOM_HPP