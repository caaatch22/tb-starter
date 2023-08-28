#include <BS_thread_pool.hpp>
#include <algorithm>
#include <execution>
#include <memory>
#include <numeric>
#include <vector>
#include "../../tbs/include/random.hpp"
#include "../../tbs/include/timer.hpp"
#include "../../tbs/include/utils.hpp"

using tbs::Timer;

void malloc_ints() {
  const int n = 1e6;
  Timer timer;
  timer.reset();
  int sum = 0;
  std::vector<int64_t*> ints(n);
  for (int i = 0; i < n; i++) {
    ints[i] = new int64_t();
  }
  std::cout << "malloc 1'000'000 ints: " << timer << '\n';
  for (int i = 0; i < n; i++) {
    sum += (*ints[i]);
  }
}

void malloc_ints_threadpool() {
  BS::thread_pool pools(8);
  constexpr int n = 1e6;
  Timer timer;

  using Data = int64_t;
  std::vector<Data*> datas(n);
  timer.reset();
  pools.push_loop(datas.size(), [&](size_t l, size_t r) {
    for (size_t i = l; i < r; i++) {
      datas[i] = new Data();
    }
  });
  pools.wait_for_tasks();
  std::cout << "malloc 1'000'000 big ints with 8 threads: " << timer << '\n';
}

void malloc_large() {
  constexpr int n = 1e6;
  Timer timer;

  using Data = tbs::BigMockData;
  std::vector<std::unique_ptr<Data>> datas(n);

  timer.reset();
  for (int i = 0; i < n; i++) {
    datas[i] = std::make_unique<Data>();
  }
  std::cout << "malloc 1'000'000 big datas: " << timer << '\n';
  int64_t sum = 0;
  for (int i = 0; i < n; i++) {
    sum += datas[i]->eam_code;
  }
}

void malloc_large2() {
  constexpr int n = 1e6;
  Timer timer;

  using Data = tbs::BigMockData;
  timer.reset();
  std::vector<Data> datas(n);
  std::cout << "malloc 1'000'000 big datas: " << timer << '\n';
  int64_t sum = 0;
  for (int i = 0; i < n; i++) {
    sum += datas[i].eam_code;
  }
}

void malloc_large_threadpool() {
  auto pools = std::make_unique<BS::thread_pool>(8);
  constexpr int n = 1e7;
  Timer timer;

  using Data = tbs::BigMockData;
  std::vector<std::unique_ptr<Data>> datas(n);
  timer.reset();
  pools->push_loop(datas.size(), [&](size_t l, size_t r) {
    for (size_t i = l; i < r; i++) {
      datas[i] = std::make_unique<Data>();
    }
  });
  pools->wait_for_tasks();
  std::cout << "malloc 1'000'000 big datas with 8 threads: " << timer << '\n';
  int64_t sum = 0;
  for (int i = 0; i < n; i++) {
    sum += datas[i]->eam_code;
  }
}

void malloc_stl_containers() {
  constexpr int n = 1e6;
  Timer timer;

  timer.reset();
  std::vector<std::string> strs(n);
  std::cout << "init vector of 1e6 strings: " << timer << '\n';

  timer.reset();
  auto dates = tbs::rng_dates(n, "10100202", "90900101");
  std::cout << "init vector of 1e6 random dates: " << timer << '\n';

  // std::sort(std::execution::par_unseq, dates.begin(), dates.end());
}

int main() {
  // for (int i = 0; i < 10; i++) {
  malloc_ints();
  malloc_large();
  // malloc_ints_threadpool();
  malloc_large_threadpool();
  malloc_stl_containers();
  // }

  return 0;
}