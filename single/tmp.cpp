#include <algorithm>
#include <chrono>
#include <concepts>
#include <execution>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <string>
#include <vector>

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

using tbs::Timer;

int64_t HDC() {
  return std::thread::hardware_concurrency();
}

int64_t LocalId() {
  static const int64_t HD = std::thread::hardware_concurrency();
  const int64_t suffix =
      ((std::hash<std::thread::id>{}(std::this_thread::get_id()) % HD) &
       0xFFLL);
  return (std::chrono::system_clock::now().time_since_epoch().count() << 8) |
         (suffix);
}

int64_t Origin() {
  return std::chrono::system_clock::now().time_since_epoch().count();
}

int main() {
  const int n = 1e5;
  std::vector<int> a(n);
  std::ranges::generate(a, rng);
  // std::sort(std::execution::par_unseq, a.begin(), a.end());
  std::ranges::sort(a);
  // std::sort(a.begin(), a.end());
  std::cout << timer << '\n';
  timer.reset();
  for (int i = 0; i < n; i++) {
    b[i] = Origin();
  }
  std::cout << timer << '\n';
  timer.reset();
  for (int i = 0; i < n; i++) {
    c[i] = HDC();
  }

  BS::thread_pool pool(32);
  for (int i = 0; i < 32; i++) {
    // std::cout << thread::hardware_concurrency() << '\n';
    auto idx = (std::hash<std::thread::id>{}(std::this_thread::get_id()) % 16);
    pool.push_task([=]() {
      std::cout << std::this_thread::get_id() << ": " << idx << '\n';
    });
  }
  pool.wait_for_tasks();

  std::cout << timer << '\n';
}
