#pragma once

#include <immintrin.h>

#include "gemm_utils.h"

// loop interchange, order: i, k, j and loop unrolling and AVX2
inline void block_avx2(Matrix& C, Matrix const& A, Matrix const& B, size_t i,
                       size_t j, size_t k) {
  // C(i, j(0, 4)) += A(i, k) * B(k, j(0, 4));
  // C(i + 1, j(0, 4)) += A(i + 1, k) * B(k, j(0, 4));
  // C(i + 2, j(0, 4)) += A(i + 2, k) * B(k, j(0, 4));
  // C(i + 3, j(0, 4)) += A(i + 3, k) * B(k, j(0, 4));
  __m256 b_vec = _mm256_loadu_ps(&B(k, j));
  for (size_t ii = i; ii < i + 8; ii++) {
    __m256 a_vec = _mm256_set1_ps(A(ii, k));
    __m256 c_vec = _mm256_loadu_ps(&C(ii, j));
    c_vec = _mm256_fmadd_ps(a_vec, b_vec, c_vec);
    // c_vec = _mm256_add_ps(c_vec, _mm256_mul_ps(a_vec, b_vec));
    _mm256_storeu_ps(&C(ii, j), c_vec);
  }
}

void gemm3(Matrix& C, Matrix const& A, Matrix const& B) {
  for (size_t i = 0; i < A.row(); i += 8) {
    for (size_t k = 0; k < A.col(); k++) {
      for (size_t j = 0; j < B.col(); j += 8) {
        block_avx2(C, A, B, i, j, k);
      }
    }
  }
}