#include "../tbs/include/thread_safe_queue.hpp"
#include <BS_thread_pool.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "../tbs/include/lockfree_ring_buffer.hpp"
#include "../tbs/include/random.hpp"
#include "../tbs/include/timer.hpp"

using std::string;
using std::vector;
using tbs::Timer;

void validTest() {
  constexpr int n = 100'000;
  Timer<tbs::micro> timer;
  ThreadSafeQueue<string> q;
  BS::thread_pool pool(8);
  vector<string> dates = tbs::rng_dates(n, "20220104", "99870107");

  timer.reset();
  pool.push_loop(dates.size(), [&](size_t l, size_t r) {
    for (size_t i = l; i < r; i++) {
      q.push(dates[i]);
    }
  });
  pool.wait_for_tasks();
  std::cout << timer << '\n';

  LockFreeRingBuffer<string> rb(n);
  timer.reset();
  pool.push_loop(dates.size(), [&](size_t l, size_t r) {
    for (size_t i = l; i < r; i++) {
      rb.push(dates[i]);
    }
  });
  pool.wait_for_tasks();

  std::cout << timer << '\n';
}

int main() {
  validTest();

  return 0;
}