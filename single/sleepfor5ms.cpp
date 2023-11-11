#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

using namespace std::literals::chrono_literals;

static const constexpr auto kInterval = 5ms;
static const constexpr auto kSpin = 100us;

int main(int argc, char* argv[]) {
  std::chrono::nanoseconds duration = 1min;
  if (argc > 1) {
    duration = std::chrono::seconds(std::atoi(argv[1]));
  }
  int ticks = duration / kInterval;
  std::vector<double> errors;
  errors.reserve(ticks);

  int digits = 0;
  do {
    ticks /= 10;
    ++digits;
  } while (ticks > 0);

  auto now = std::chrono::high_resolution_clock::now();
  auto begin = now;
  auto prev = now;
  auto tick = now;
  auto final = now + duration + kInterval;
  begin += kInterval;  // 多执行一次作为预热
  int i = 0;
  while (now < final) {
    prev = now;
    tick += kInterval;
    std::this_thread::sleep_until(tick - kSpin);
    now = std::chrono::high_resolution_clock::now();
    while (now < tick) {
      std::this_thread::yield();
      now = std::chrono::high_resolution_clock::now();
    }
    auto error =
        std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(
            now - tick);
    errors.push_back(error.count());
    ++i;
  }
  errors.erase(errors.begin());  // 统计时丢弃第一个预热tick

  std::ios::sync_with_stdio(false);
  std::cout.tie(nullptr);
  std::cout.precision(3);
  std::cout.setf(std::ios::fixed);
  double std_err = 0.0;
  double max_err = 0.0;
  for (size_t i = 0; i < errors.size(); ++i) {
    std_err += std::pow(errors.at(i), 2);
    max_err = std::max(std::abs(max_err), std::abs(errors.at(i)));
    std::cout << "tick " << std::setw(digits) << (i + 1)
              << " error: " << std::setw(8) << errors.at(i) << "us"
              << std::endl;
  }
  std_err = std::sqrt(std_err / errors.size());
  for (double& error : errors) {
    error = std::abs(error);
  }
  std::sort(errors.begin(), errors.end());
  double most_95 = errors.at(errors.size() * 0.95);
  double most_99 = errors.at(errors.size() * 0.99);
  auto elapsed =
      std::chrono::duration_cast<std::chrono::duration<double>>(now - begin);
  std::cout << "total " << errors.size() << " ticks in " << elapsed.count()
            << "s" << std::endl;
  std::cout << "std error: " << std_err << "us" << std::endl;
  std::cout << "max error: " << max_err << "us" << std::endl;
  std::cout << "95% error: " << most_95 << "us" << std::endl;
  std::cout << "99% error: " << most_99 << "us" << std::endl;

  return 0;
}