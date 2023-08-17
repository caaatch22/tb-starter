#ifndef BENCH_TIMER_HPP
#define BENCH_TIEMR_HPP

#include <chrono>
#include <concepts>

namespace tbs {

using nano = std::chrono::nanoseconds;
using micro = std::chrono::microseconds;
using mill = std::chrono::milliseconds;
using sec = std::chrono::seconds;
using min = std::chrono::minutes;
using hour = std::chrono::hours;

template <typename D>
concept Dur = std::is_same_v<D, nano> or std::is_same_v<D, micro> or
              std::is_same_v<D, mill> or std::is_same_v<D, sec> or
              std::is_same_v<D, min> or std::is_same_v<D, hour>;

template <Dur dur = mill>
class Timer {
 public:
  Timer() : start_time_(std::chrono::high_resolution_clock::now()) {}

  void reset() { start_time_ = std::chrono::high_resolution_clock::now(); }

  double elapsed() const {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<dur>(end_time - start_time_);
    return duration.count();
  }

  friend std::ostream& operator<<(std::ostream& os, Timer const& timer) {
    if constexpr (std::is_same_v<dur, nano>) {
      os << timer.elapsed() << " ns";
    } else if constexpr (std::is_same_v<dur, micro>) {
      os << timer.elapsed() << " us";
    } else if constexpr (std::is_same_v<dur, mill>) {
      os << timer.elapsed() << " ms";
    } else if constexpr (std::is_same_v<dur, sec>) {
      os << timer.elapsed() << " s";
    } else if constexpr (std::is_same_v<dur, min>) {
      os << timer.elapsed() << " min";
    } else if constexpr (std::is_same_v<dur, hour>) {
      os << timer.elapsed() << " h";
    }
    return os;
  }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
};

}  // namespace tbs

#endif  // BENCH_TIMER_HPP