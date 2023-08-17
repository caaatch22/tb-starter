#ifndef BENCH_UTILS_HPP
#define BENCH_UTILS_HPP

#include <charconv>
#include <chrono>
#include <string_view>
#include <system_error>
#include <tl/expected.hpp>

namespace bench {

tl::expected<int, std::errc> svtoi(string_view sv) {
  int value;
  auto [_, rc] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
  if (rc == std::errc{}) {
    return tl::unexpected{rc};
  }
  return value;
}

tl::expected<long, std::errc> svtol(string_view sv) {
  long value;
  auto [_, rc] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
  if (rc == std::errc{}) {
    return tl::unexpected{rc};
  }
  return value;
}

std::chrono::year_month_day svtoymd(string_view sv) {
  auto const yi = svtoi(sv.substr(0, 4)).value();
  auto const mi = svtoi(sv.substr(0, 4)).value();
  auto const di = svtoi(sv.substr(0, 4)).value();
  return ymd{std::chrono::year(yi), std::chrono::month(mi),
             std::chrono::day(di)};
};

struct Non

}  // namespace bench

#endif  // BENCH_UTILS_HPP