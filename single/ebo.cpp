#include <bits/stdc++.h>

struct A {};

struct B {
  int a;
  A emp;
};

struct C : public A {
  int b;
};

struct D {
  int a;
  [[no_unique_address]] A emp1;
};

struct E {
  int a;
  [[no_unique_address]] A emp1;
  [[no_unique_address]] A emp2;
};

struct F {
  int a;
  [[no_unique_address]] A emp1;
  [[no_unique_address]] A emp2;
  [[no_unique_address]] A emp3;
};

int main() {
  static_assert(sizeof(A) == 1);
  static_assert(sizeof(B) == 8);
  static_assert(sizeof(C) == 4);
  static_assert(sizeof(D) == 4);
  static_assert(sizeof(E) == 8);
  static_assert(sizeof(F) == 8);

  std::vector<int> a{1, 2, 1, 1};
  std::vector<int> b{1, 2, 1, 1};

  std::copy(a.begin(), a.end(), b.begin());

  return 0;
}