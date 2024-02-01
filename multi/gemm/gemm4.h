#pragma once
#include <immintrin.h>

#include "gemm_utils.h"

inline void block_avx2_more(Matrix& C, Matrix& A, Matrix& B, size_t i, size_t j,
                            size_t k) {
  //   for (int p = 0; p < 4; p++) {
  //     C(i + p, j(0, 4)) += A(i + p, k) * B(k, j(0, 4));
  //     C(i + p, j(4, 8)) += A(i + p, k) * B(k, j(4, 8));
  //     C(i + p, j(8, 12)) += A(i + p, k) * B(k, j(8, 12));
  //     C(i + p, j(12, 16)) += A(i + p, k) * B(k, j(12, 16));
  //   }

  for (int jj = 0; jj < 4; jj++) {
    __m256d b_vec = _mm256_loadu_pd(&B(k, j + jj * 4));
    for (int ii = 0; ii < 4; ii++) {
      __m256d a_vec = _mm256_set1_pd(A(i + ii, k));
      __m256d c_vec = _mm256_loadu_pd(&C(i + ii, j + jj * 4));
      c_vec = _mm256_add_pd(c_vec, _mm256_mul_pd(a_vec, b_vec));
      _mm256_storeu_pd(&C(i + ii, j + jj * 4), c_vec);
    }
  }

  //   for (int ii = 0; ii < 4; ii++) {
  //     __m256d a_vec = _mm256_set1_pd(A(i + ii, k));
  //     __m256d c_vec = _mm256_loadu_pd(&C(i + ii, j));
  //     c_vec = _mm256_add_pd(c_vec, _mm256_mul_pd(a_vec, b_vec));
  //     _mm256_storeu_pd(&C(i + ii, j), c_vec);
  //   }
}

void gemm4(Matrix& C, Matrix& A, Matrix& B) {
#pragma omp parallel for
  for (size_t i = 0; i < A.row(); i += 4) {
    for (size_t j = 0; j < B.col(); j += 16) {
      for (size_t k = 0; k < B.row(); k++) {
        block_avx2_more(C, A, B, i, j, k);
      }
    }
  }
}