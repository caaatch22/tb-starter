#ifndef TBS_UTILS_HPP
#define TBS_UTILS_HPP

#include <date/date.h>
#include <fmt/format.h>
#include <charconv>
#include <string>
#include <string_view>
#include <system_error>
#include <tl/expected.hpp>
#include <vector>

namespace tbs {

using YMD = date::year_month_day;

tl::expected<int, std::errc> svtoi(std::string_view sv, int base = 10) {
  int value;
  auto const [_, rc] =
      std::from_chars(sv.data(), sv.data() + sv.size(), value, base);
  if (rc != std::errc{}) [[unlikely]] {
    return tl::unexpected{rc};
  }
  return value;
}

tl::expected<long, std::errc> svtol(std::string_view sv, int base = 10) {
  long value;
  auto [_, rc] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
  if (rc != std::errc{}) [[unlikely]] {
    return tl::unexpected{rc};
  }
  return value;
}

YMD svtoymd(std::string_view sv) {
  auto const yi = svtoi(sv.substr(0, 4)).value();
  auto const mi = svtoi(sv.substr(4, 2)).value();
  auto const di = svtoi(sv.substr(6, 2)).value();
  return YMD{date::year(yi), date::month(mi), date::day(di)};
};

std::string ymdtostr(YMD ymd) {
  return fmt::format("{:4d}{:2d}{:2d}", ymd.year().operator int(),
                     ymd.month().operator unsigned int(),
                     ymd.day().operator unsigned int());
}

/// mock Test struct
struct MockData {
  std::string symbol{"000001.SZ"};
  int64_t eamcode{13000000000020020LL};
  MockData() = default;
  explicit MockData(std::string smb, int64_t code)
      : symbol(std::move(smb)), eamcode(code) {}
};

struct NonCopy {
  std::string symbol{"000001.SZ"};
  int64_t eamcode{13000000000020020LL};
  NonCopy() = default;

  NonCopy(NonCopy const&) = delete;
  NonCopy& operator=(NonCopy const&) = delete;
};

struct NonMove {
  std::string symbol{"000001.SZ"};
  int64_t eamcode{13000000000020020LL};
  NonMove() = default;

  NonMove(NonMove&&) = delete;
  NonMove& operator=(NonMove&&) = delete;
};

struct NonCopyAndNonMove {
  std::string symbol{"000001.SZ"};
  int64_t eamcode{13000000000020020LL};
  NonCopyAndNonMove() = default;

  NonCopyAndNonMove(NonCopyAndNonMove const&) = delete;
  NonCopyAndNonMove(NonCopyAndNonMove&&) = delete;
  NonCopyAndNonMove& operator=(NonCopyAndNonMove const&) = delete;
  NonCopyAndNonMove& operator=(NonCopyAndNonMove&&) = delete;
};

}  // namespace tbs

#endif  // TBS_UTILS_HPP