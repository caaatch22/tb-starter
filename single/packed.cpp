#include <bits/stdc++.h>

struct tmp1 {
  char a1[13];
  char a2[5];
};  // 13+5

struct __attribute__((packed)) tmp2 {
  char a1[13];
  int c;
  char a2[5];
};

struct tmp3 {
  char a1[13];
  int c;
  char a2[5];
};

struct tmp4 {
  char a1[13];
  char a2[5];
  char c;
  double a;
};

int main() {
  std::cout << sizeof(tmp1) << '\n';
  std::cout << sizeof(tmp2) << '\n';
  std::cout << sizeof(tmp3) << '\n';
  std::cout << sizeof(tmp4) << '\n';
}