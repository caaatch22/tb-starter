#ifndef TBS_UTILS_HPP
#define TBS_UTILS_HPP

#include <charconv>
#include <date/date.h>
#include <string_view>
#include <format>
#include <system_error>
#include <tl/expected.hpp>

namespace tbs {

using YMD = date::year_month_day;

tl::expected<int, std::errc> svtoi(std::string_view sv) {
  int value;
  auto const [_, rc] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
  if (rc == std::errc{}) {
    return tl::unexpected{rc};
  }
  return value;
}

tl::expected<long, std::errc> svtol(std::string_view sv) {
  long value;
  auto [_, rc] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
  if (rc == std::errc{}) {
    return tl::unexpected{rc};
  }
  return value;
}

YMD svtoymd(std::string_view sv) {
  auto const yi = svtoi(sv.substr(0, 4)).value();
  auto const mi = svtoi(sv.substr(4, 2)).value();
  auto const di = svtoi(sv.substr(6, 2)).value();
  return YMD{date::year(yi), date::month(mi),
             date::day(di)};
};

std::string ymdtostr(YMD ymd) {
  return std::format("{:4d}{:2d}{:2d}", ymd.year().operator int(), ymd.month().operator unsigned int(), ymd.day().operator unsigned int());
}

}  // namespace tbs

#endif  // TBS_UTILS_HPP