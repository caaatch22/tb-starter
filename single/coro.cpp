#include <coroutine>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <variant>

template <typename T>
struct [[nodiscard]] generator {
  struct promise_type {
    auto get_return_object() noexcept { return generator{*this}; }
    std::suspend_always initial_suspend() const noexcept { return {}; }
    std::suspend_always final_suspend() const noexcept { return {}; }
    std::suspend_always yield_value(const T& value) noexcept(
        std::is_nothrow_copy_constructible_v<T>) {
      result = value;
      return {};
    }
    std::suspend_always yield_value(T&& value) noexcept {
      result = std::addressof(value);
      return {};
    }

    void return_void() const noexcept {}
    void unhandled_exception() noexcept(
        std::is_nothrow_copy_constructible_v<std::exception_ptr>) {
      result = std::current_exception();
    };

    T& getValue() {
      if (hasException())
        std::rethrow_exception(std::get<std::exception_ptr>(result));
      return std::holds_alternative<T>(result) ? std::get<T>(result)
                                               : *std::get<T*>(result);
    }

    bool hasException() const noexcept {
      return std::holds_alternative<std::exception_ptr>(result);
    }

   private:
    std::variant<std::monostate, T, T*, std::exception_ptr> result;
  };

  generator(generator&& rhs) noexcept
      : coro{std::exchange(rhs.coro, nullptr)} {}
  generator& operator=(generator&& rhs) noexcept {
    if (coro)
      coro.destroy();
    coro = std::exchange(rhs.coro, nullptr);
  }
  ~generator() {
    if (coro)
      coro.destroy();
  }

  auto& operator()() const {
    coro();  // same as 'coro.resume();'
    return coro.promise().getValue();
  }
  bool advance() const noexcept {
    coro();
    return !coro.done() || coro.promise().hasException();
  }
  bool hasException() const noexcept { return coro.promise().hasException(); }
  auto& getValue() const { return coro.promise().getValue(); }

 private:
  explicit generator(promise_type& promise) noexcept
      : coro{std::coroutine_handle<promise_type>::from_promise(promise)} {}

  std::coroutine_handle<promise_type> coro;
};

generator<std::string> foo() {
  co_yield "hello";
  const auto s = std::string{"world"};
  co_yield s;
}

int main() {
  const auto f = foo();
  std::cout << f() << ' ' << f() << '\n';
  std::cout << f() << '\n';
  return 0;
}

// 1.const auto f = foo();
// (1) 编译器：生成一个coroutine frame，主要包括promise,
// 根据你提供的promise_type相关函数生成
// (2) 编译器调用 get_return_object()，返回一个 generator{*this};
// (3) 编译器调用 initial_suspend()， 返回awaitables，
// 标准库提供的awaitables包括 suspend_always,
// suspend_never,这里我们返回suspend_always
// 2. 执行cout语句，执行第一个f()
// --> 执行auto& operator()() const
// --> coro.resume();
// 3. 进入foo()，执行co_yield "hello";
// (4) 执行promise中的yield_value()，将yield的值放在result中，返回suspend_always
// 4. 程序resume到auto& opeator()()中，执行coro.promise().getValue();
// 一场会被保存在promise内，看你怎么处理
// 5. 回到主函数，打印‘ ’，再次执行f()
// --> 执行auto& operator()() const
// --> coro.resume();
// 6. 进入foo()，执行co_yield s;
// (5) 执行promise中的yield_value()，将yield的值放在result中，返回suspend_always