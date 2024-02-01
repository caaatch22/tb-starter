#pragma once
#include <immintrin.h>

#include "gemm_utils.h"

inline void block_avx2(Matrix& C, Matrix& A, Matrix& B, size_t i, size_t j,
                       size_t k) {
  // C(i, j(0, 4)) += A(i, k) * B(k, j(0, 4));
  // C(i + 1, j(0, 4)) += A(i + 1, k) * B(k, j(0, 4));
  // C(i + 2, j(0, 4)) += A(i + 2, k) * B(k, j(0, 4));
  // C(i + 3, j(0, 4)) += A(i + 3, k) * B(k, j(0, 4));
  __m256d b_vec = _mm256_loadu_pd(&B(k, j));
  for (int ii = 0; ii < 4; ii++) {
    __m256d a_vec = _mm256_set1_pd(A(i + ii, k));
    __m256d c_vec = _mm256_loadu_pd(&C(i + ii, j));
    c_vec = _mm256_add_pd(c_vec, _mm256_mul_pd(a_vec, b_vec));
    _mm256_storeu_pd(&C(i + ii, j), c_vec);
  }
}

void gemm3(Matrix& C, Matrix& A, Matrix& B) {
  for (size_t i = 0; i < A.row(); i += 4) {
    for (size_t j = 0; j < B.col(); j += 4) {
      for (size_t k = 0; k < B.row(); k++) {
        block_avx2(C, A, B, i, j, k);
      }
    }
  }
}