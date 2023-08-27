#include <iostream>
#include <vector>
// #define CATCH_CONFIG_MAIN
// #include <catch2/catch.hpp>
#include "../../tbs/include/timer.hpp"
#include "../../tbs/include/utils.hpp"
#include "../../tbs/include/random.hpp"
#include "../../tbs/include/proxy.h"
#include <ranges>
#include <memory>
#include <algorithm>

using std::vector;
using tbs::rng;
using tbs::Timer;

// use proxy 
struct Addable : pro::dispatch<int(int)> {
  template <class T>
  int operator()(T& self, int rhs) {
    return self.Add(rhs);
  }
};

struct Mulable : pro::dispatch<int(int)> {
  template <class T>
  int operator()(T& self, int rhs) {
    return self.Mul(rhs);
  }
};

struct AddMulFacade : pro::facade<Addable, Mulable> {};

struct Integer {
  int a;
  int Add(int b) { return a += b;}
  int Mul(int b) { return a *= b;}
};

// double Consume(pro::proxy<AddMulFacade> p) {
//   std::stringstream result;
//   result << "shape = ";
//   p.invoke<Draw>(result);
//   result << ", area = " << p.invoke<Area>();
//   return std::move(result).str();
// }

// Client - Producer
// pro::proxy<DrawableFacade> CreateRectangleAsDrawable(int width, int height) {
//   Rectangle rect;
//   rect.SetWidth(width);
//   rect.SetHeight(height);
//   return pro::make_proxy<DrawableFacade>(rect);
// }

/// use virtual function
class BaseVir {
  virtual int mul(int) = 0;
  virtual int add(int) = 0;
};

class DerivedVir : BaseVir {
public:
  int a{1114514};
  int mul(int b) { return a *= b; }
  int add(int b) { return a += b; }
};


/// use CRTP
template <typename T>
class BaseCRTP {
  void mul() { static_cast<T*>(this)->mul(); }
  void add() { static_cast<T*>(this)->add(); }
};

class DerivedCRTP : public BaseCRTP<DerivedCRTP> {
public:  
  int a{114514};
  int add(int b) { return a += b; }
  int mul(int b) { return a *= b; }
};


void test1() {
  Timer timer;
  constexpr int n = 1e8;
  auto dummy = 1;
  int cnt = 0;
  timer.reset();
  for (int i = 0; i < n; i++) {
    dummy += cnt++;
  }
   for (int i = 0; i < n; i++) {
    dummy *= cnt++;
  }
  std::cout << "dummy: " << timer << " " << dummy << '\n';

  auto a = std::make_unique<DerivedVir>();  
  auto res1 = 1;
  timer.reset();
  for (int i = 0; i < n; i++) {
    res1 += a->add(rng());
  }
  for (int i = 0; i < n; i++) {
    res1 *= a->mul(rng());
  }
  std::cout << "virtual: " << timer << " " << res1 << '\n';

  auto res2 = 1;
  auto b = std::make_unique<DerivedCRTP>();
  timer.reset();
  for (int i = 0; i < n; i++) {
    res2 += a->add(rng());
  }
  for (int i = 0; i < n; i++) {
    res2 *= a->mul(rng());
  }
  std::cout << "CRTP: " << timer << " " << res2 << '\n';

  Integer cc;
  auto c = pro::make_proxy<AddMulFacade>(cc);
  int res3 = 1;
  timer.reset();
  for (int i = 0; i < n; i++) {
    res3 += c.invoke<Addable>(rng());
  }
  for (int i = 0; i < n; i++) {
    res3 += c.invoke<Mulable>(rng());
  }
  std::cout << "Proxy: " << timer << " " << res3 << '\n';
}

int main() {
  test1();

  return 0;
}