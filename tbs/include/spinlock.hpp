#ifndef TBS_SPINLOCK
#define TBS_SPINLOCK

#include <time.h>
#include <atomic>

namespace tbs {

class spinlock {
 public:
  void lock() {
    static constexpr timespec ns{0, 1};
    for (int i = 0; flg_.load(std::memory_order_relaxed) ||
                    flg_.exchange(1, std::memory_order_acquire);
         i++) {
      if (i == 8) {
        i = 0;
        nanosleep(&ns, NULL);
        // sched_yield();
      }
    }
  }

  void unlock() { flg_.store(0, std::memory_order_release); }

 private:
  std::atomic<unsigned int> flg_;
};

}  // namespace tbs

#endif  // TBS_SPINLOCK