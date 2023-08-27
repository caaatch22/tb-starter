#include <iostream>
#include <memory>
#include <memory_resource>
#include <ranges>
#include <string_view>
#include "../tbs/include/pmr_unique_ptr.hpp"
#include "../tbs/include/random.hpp"
#include "../tbs/include/timer.hpp"
#include "../tbs/include/utils.hpp"

// #define CATCH_CONFIG_MAIN
// #include "catch2/catch.hpp"

using namespace std;
using tbs::Timer;
using Data = tbs::MockData;

template <typename T>
using PMR_UP = tbs::polymorphic_unique_ptr<T>;

template <typename T>
using UP = std::unique_ptr<T>;

void test1() {
  Timer timer;
  const int n = 10000000;
  //   std::array<std::byte, n * sizeof(Data)> buf;
  std::vector<std::byte> buf(n * sizeof(Data));
  timer.reset();
  //   std::pmr::unsynchronized_pool_resource mbr;
  std::pmr::monotonic_buffer_resource mbr(buf.data(), buf.size());
  pmr::polymorphic_allocator<Data> pmr_alloc(&mbr);
  vector<PMR_UP<Data>> pmr_p;
  timer.reset();
  pmr_p.reserve(n);
  for (int i = 0; i < n; i++) {
    pmr_p.emplace_back(tbs::make_polymorphic_unique<Data>(&mbr));
  }
  std::cout << "sizeof Data: " << sizeof(Data) << '\n';
  std::cout << "use pmr_alloc: " << timer << '\n';
  for (int i = 0; i < 10; i++) {
    std::cout << &pmr_p[i] << ' ' << pmr_p[i].get() << '\n';
  }

  timer.reset();
  std::vector<UP<Data>> p(n);
  timer.reset();
  for (int i = 0; i < n; i++) {
    p[i] = std::make_unique<Data>();
  }
  std::cout << "use make_unique: " << timer << '\n';
  for (int i = 0; i < 10; i++) {
    std::cout << &p[i] << ' ' << p[i].get() << '\n';
  }
}

void test2() {
  cout << "contagious" << '\n';
  std::pmr::vector<int>(1090);
  std::array<std::byte, 4096> buf;
  std::pmr::monotonic_buffer_resource(buf.data(), buf.size());
}

int main() {
  test1();
}