#ifndef BENCH_TIMER_HPP
#define BENCH_TIEMR_HPP

#include <fmt/format.h>

#include <chrono>
#include <concepts>

#include "mp.hpp"

namespace tbs {

class Timer {
 public:
  Timer() noexcept : start(std::chrono::high_resolution_clock::now()) {}

  [[nodiscard]] auto elapsed() const noexcept -> std::chrono::duration<double> {
    const auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(now - start);
  }

  void reset() noexcept { start = std::chrono::high_resolution_clock::now(); }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

// class Timer {
//  public:
//   Timer() noexcept { rdtsc(); }

//   [[nodiscard]] auto operator<=>(const Timer &rhs) const noexcept {
//     return qw <=> rhs.qw;
//   }

//   [[nodiscard]] auto elapsed() const noexcept ->
//   std::chrono::duration<double> {
//     Timer now;
//     return std::chrono::duration<double>((now.qw - qw) / 1e9);
//   }

//   void reset() noexcept { rdtsc(); }

//  private:
//   union {
//     uint32_t dw[2];
//     uint64_t qw;
//   };

//   inline void rdtsc() noexcept {
//     asm volatile("rdtsc\n\t" : "=a"(dw[0]), "=d"(dw[1]));
//   }
// };

}  // namespace tbs

template <>
struct fmt::formatter<tbs::Timer> : formatter<double> {
  template <typename FormatContext>
  auto format(const tbs::Timer &timer, FormatContext &ctx) const
      -> decltype(ctx.out()) {
    return fmt::formatter<double>::format(timer.elapsed().count(), ctx);
  }
};

#endif  // BENCH_TIMER_HPP