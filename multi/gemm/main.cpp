#include <fmt/format.h>

#include <string>
#include <vector>

#include "gemm0.h"
#include "gemm1.h"
#include "gemm2.h"
#include "gemm3.h"
#include "gemm4.h"
#include "gemm5.h"
#include "gemm6.h"

constexpr int M = 10000;
constexpr int K = 10000;
constexpr int N = 10000;

void check_equal(Matrix const& A, Matrix const& B) {
  if (A != B) {
    for (size_t i = 0; i < 3; i++) {
      for (size_t j = 0; j < 3; j++) {
        fmt::print("A[{}][{}] = {}, B[{}][{}] = {}\n", i, j, A(i, j), i, j,
                   B(i, j));
      }
    }
    throw std::runtime_error("A != B");
  }
}

Matrix run(Matrix& C, Matrix const& A, Matrix const& B, auto&& gemm,
           int version, tbs::Timer& timer) {
  timer.reset();
  C.zeros();
  gemm(C, A, B);
  double const time = timer.elapsed().count();

  fmt::print("gemm{}: {:.3f} s, GFlops: {:.3f}\n", version, time,
             GFlops(time, M, N, K));
  return C;
}

int main() {
  Matrix A(M, K);
  Matrix B(K, N);
  Matrix C(M, N);
  fmt::print("M: {}, K: {}, N: {}\n", M, K, N);
  C.zeros();
  A.random();
  B.random();

  tbs::Timer timer;
  // auto const C0 = run(C, A, B, gemm0, 0, timer);
  // auto const C1 = run(C, A, B, gemm1, 1, timer);
  // check_equal(C0, C1);
  // auto const C2 = run(C, A, B, gemm2, 2, timer);
  // check_equal(C1, C2);
  // auto const C3 = run(C, A, B, gemm3, 3, timer);
  // check_equal(C0, C3);
  // auto const C4 = run(C, A, B, gemm4, 4, timer);
  // check_equal(C1, C4);

  auto const C5 = run(C, A, B, gemm5, 5, timer);
  // check_equal(C5, C1);
  auto const C6 = run(C, A, B, gemm6, 6, timer);
  // try {
  check_equal(C6, C5);
  // } catch (std::exception const& e) {
  //   fmt::print("C6 != C1\n");
  // }

  // check_equal(C1, C6);

  // for (size_t i = 0; i < 3; i++) {
  //   for (size_t j = 0; j < 3; j++) {
  //     fmt::print("C1[{}][{}] = {}, C5[{}][{}] = {}\n", i, j, C1(i, j), i, j,
  //                C5(i, j));
  //   }
  // }

  return 0;
}
