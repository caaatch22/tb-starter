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

struct BigMockData {
  int64_t epoch_local_time = 0;
  std::string symbol{};
  std::string market{};
  uint32_t stream_id = 0;
  int64_t epoch_exchg_time = 0;
  uint16_t channel = 0;
  int32_t status = 0;
  double pre_close = 0.0;
  double open = 0.0;
  double high = 0.0;
  double low = 0.0;
  double match = 0.0;
  double close = 0.0;
  std::vector<double> ask_px{};
  std::vector<double> bid_px{};
  std::vector<int64_t> ask_vol{};
  std::vector<int64_t> bid_vol{};
  double upper_limit = 0.0;
  double lower_limit = 0.0;
  uint64_t volume = 0;
  double amount = 0.0;
  double settle_px = 0.0;
  double pre_settle_px = 0.0;
  int64_t interest = 0;
  int64_t pre_interest = 0;
  int32_t num_trades = 0;
  int64_t total_bid_vol = 0;
  int64_t total_ask_vol = 0;
  double weighted_avg_bid_px = 0.0;
  double weighted_avg_ask_px = 0.0;
  uint32_t local_time = 0;
  uint32_t exchg_time = 0;
  int64_t trade_date = 0;
  int32_t key = -1;
  int64_t eam_code = 0;
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