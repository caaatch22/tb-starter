#ifndef TBS_UTILS_HPP
#define TBS_UTILS_HPP

#include <date/date.h>
#include <fmt/format.h>

#include <charconv>
#include <concepts>
#include <string>
#include <string_view>
#include <system_error>
#include <tl/expected.hpp>
#include <vector>

namespace tbs {

using YMD = date::year_month_day;

template <std::integral T>
tl::expected<T, std::errc> svtoi(std::string_view sv, int base = 10) {
  T value;
  auto const [_, rc] =
      std::from_chars(sv.data(), sv.data() + sv.size(), value, base);
  if (rc != std::errc{}) [[unlikely]] {
    return tl::unexpected{rc};
  }
  return value;
}

YMD svtoymd(std::string_view sv) {
  auto const yi = svtoi<int>(sv.substr(0, 4)).value();
  auto const mi = svtoi<int>(sv.substr(4, 2)).value();
  auto const di = svtoi<int>(sv.substr(6, 2)).value();
  return YMD{date::year(yi), date::month(mi), date::day(di)};
};

std::string ymdtostr(YMD ymd) {
  return fmt::format("{:4d}{:2d}{:2d}", ymd.year().operator int(),
                     ymd.month().operator unsigned int(),
                     ymd.day().operator unsigned int());
}

}  // namespace tbs

#endif  // TBS_UTILS_HPP