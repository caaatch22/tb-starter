#include "omega_exception.hpp"

class MyException : public tbs::OmegaException<int> {
 public:
  MyException(
      const std::string& str, int data,
      const std::source_location& loc = std::source_location::current(),
      boost::stacktrace::stacktrace trace = boost::stacktrace::stacktrace())
      : tbs::OmegaException<int>(str, data, loc, trace) {}
};

[[__gnu__::__noinline__]] void func4() { throw MyException("my exception", 1); }
[[__gnu__::__noinline__]] void func3() { func4(); }
[[__gnu__::__noinline__]] void func2() { func3(); }
[[__gnu__::__noinline__]] void func1() { func2(); }

int main() {
  try {
    func1();
  } catch (const MyException& e) {
    std::cout << e.what() << '\n';
    std::cout << e.where() << '\n';
    std::cout << e.stack() << '\n';
    std::cout << e.data() << '\n';
  }

  return 0;
}