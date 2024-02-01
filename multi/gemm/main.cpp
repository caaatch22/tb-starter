#include <fmt/format.h>

#include <string>
#include <vector>

#include "gemm0.h"
#include "gemm1.h"
#include "gemm2.h"
#include "gemm3.h"
#include "gemm4.h"

constexpr int M = 1600;
constexpr int K = 1600;
constexpr int N = 1600;

void check_equal(const Matrix& A, const Matrix& B) {
  if (A == B) {
    fmt::print("OK\n");
  } else {
    fmt::print("NG\n");
  }
}

int main() {
  Matrix A(M, K);
  Matrix B(K, N);
  Matrix C(M, N);
  tbs::Timer timer;

  C.zeros();
  A.random();
  B.random();

  // gemm0(C, A, B);
  // fmt::print("gemm0: {} s\n", timer);
  // Matrix C0(C);

  // timer.reset();

  // C.zeros();
  // timer.reset();
  // gemm1(C, A, B);
  // fmt::print("gemm1: {} s\n", timer);
  // Matrix C1(C);

  // check_equal(C0, C1);

  C.zeros();
  timer.reset();
  gemm2(C, A, B);
  fmt::print("gemm2: {} s\n", timer);
  Matrix C2(C);
  // check_equal(C0, C2);

  C.zeros();
  timer.reset();
  gemm3(C, A, B);
  fmt::print("gemm3: {} s\n", timer);
  Matrix C3(C);
  // check_equal(C0, C3);

  C.zeros();
  timer.reset();
  gemm4(C, A, B);
  fmt::print("gemm4: {} s\n", timer);
  Matrix C4(C);
  // check_equal(C0, C4);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; ++j) {
      fmt::print("{} ", C4(i, j));
    }
    fmt::print("\n");
  }
  return 0;
}