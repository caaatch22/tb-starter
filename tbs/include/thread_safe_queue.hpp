#ifndef GSF2_UTILS_ThreadSafeQUEUE_HPP
#define GSF2_UTILS_ThreadSafeQUEUE_HPP

#include <mutex>
#include <optional>
#include <queue>

template <typename T>
class ThreadSafeQueue {
 private:
  std::queue<T> queue_;
  mutable std::mutex mutex_;

 public:
  ThreadSafeQueue() = default;
  ThreadSafeQueue(const ThreadSafeQueue<T>&) = delete;
  ThreadSafeQueue& operator=(const ThreadSafeQueue<T>&) = delete;

  virtual ~ThreadSafeQueue() = default;

  unsigned long size() const { return queue_.size(); }

  std::optional<T> pop() {
    std::lock_guard lock(mutex_);
    if (queue_.empty()) {
      return {};
    }

    T tmp = queue_.front();
    queue_.pop();
    return tmp;
  }

  bool try_pop(T& value) {
    std::lock_guard lock(mutex_);
    if (queue_.empty()) {
      return false;
    }

    value = queue_.front();
    queue_.pop();
    return true;
  }

  void push(const T& item) {
    std::lock_guard lock(mutex_);
    queue_.push(item);
  }

  bool empty() const { return queue_.empty(); }
};

#endif  // GSF2_UTILS_ThreadSafeQUEUE_HPP
