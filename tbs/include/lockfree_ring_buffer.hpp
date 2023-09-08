#ifndef SRC_LOCKFREERINGBUFFER_HPP
#define SRC_LOCKFREERINGBUFFER_HPP
#include <atomic>
#include <chrono>
#include <mutex>
#include <ranges>
#include <vector>

class spinlock {
  // using namespace std::literals;

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

template <typename T>
class LockFreeRingBuffer {
 public:
  LockFreeRingBuffer() : data_(kInitialSize) {
    head_.store(0, std::memory_order_relaxed);
    tail_.store(0, std::memory_order_relaxed);
  }
  explicit LockFreeRingBuffer(size_t capacity) : data_(capacity) {
    head_.store(0, std::memory_order_relaxed);
    tail_.store(0, std::memory_order_relaxed);
  }

  void push(const T& value) {
    size_t tail = tail_.load(std::memory_order_relaxed);
    size_t next_tail = next(tail);
    if (next_tail == head_.load(std::memory_order_acquire)) {
      lk_.lock();
      expand();
      next_tail = next(tail);
      lk_.unlock();
    }
    data_[tail] = value;
    tail_.store(next_tail, std::memory_order_release);
  }

  bool pop(T& value) {
    size_t head = head_.load(std::memory_order_relaxed);
    if (head == tail_.load(std::memory_order_acquire)) {
      return false;
    }
    value = std::move(data_[head]);
    head_.store(next(head), std::memory_order_release);
    return true;
  }

  bool empty() const {
    return head_.load(std::memory_order_acquire) ==
           tail_.load(std::memory_order_acquire);
  }

  size_t size() const {
    size_t head = head_.load(std::memory_order_acquire);
    size_t tail = tail_.load(std::memory_order_acquire);
    return tail >= head ? tail - head : head - tail;
  }

 private:
  static const size_t kInitialSize = 16;

  size_t next(size_t current) const { return (current + 1) % data_.size(); }

  void expand() {
    std::vector<T> next_data(data_.size() * 2);
    size_t head = head_.load(std::memory_order_acquire);
    size_t tail = tail_.load(std::memory_order_acquire);
    if (tail >= head) {
      std::ranges::move(data_.begin() + head, data_.begin() + tail,
                        next_data.begin());
    } else {
      std::ranges::move(data_.begin() + head, data_.end(), next_data.begin());
      std::ranges::move(data_.begin(), data_.begin() + tail,
                        next_data.begin() + data_.size() - head);
    }
    head_.store(0, std::memory_order_relaxed);
    tail_.store(data_.size(), std::memory_order_relaxed);
    data_ = std::move(next_data);
  }

  std::atomic<size_t> head_;
  std::atomic<size_t> tail_;
  std::vector<T> data_;
  spinlock lk_;
  // std::mutex lk_;
};

#endif /* SRC_LOCKFREERINGBUFFER_HPP */
