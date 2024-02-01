#include <algorithm>
#include <atomic>
#include <mutex>
#include <vector>

/// @brief  单例模式

/// @brief  meyer's singleton
/// thread-safe
class Singleton {
 public:
  static Singleton &getInstance() {
    static Singleton instance;
    return instance;
  }
  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;
  Singleton(Singleton &&) = delete;
  Singleton &operator=(Singleton &&) = delete;

 private:
  Singleton() = default;
  ~Singleton() = default;
};

/// @brief  call_once

class Singleton2 {
 public:
  static Singleton2 &getInstance() {
    std::call_once(flag_, create_instance);
    return *instance_;
  }

  Singleton2(const Singleton2 &) = delete;
  Singleton2 &operator=(const Singleton2 &) = delete;
  Singleton2(Singleton2 &&) = delete;
  Singleton2 &operator=(Singleton2 &&) = delete;

 private:
  Singleton2() = default;
  ~Singleton2() = default;

  static void create_instance() { instance_ = new Singleton2(); }
  static Singleton2 *instance_;
  static std::once_flag flag_;
};

///

int main() { return 0; }