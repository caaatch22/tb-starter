#pragma once

#include <immintrin.h>
#include <omp.h>

#include "gemm3.h"
#include "gemm_utils.h"

// loop interchange, order: i, k, j and loop unrolling and AVX2 and OpenMP
void gemm4(Matrix& C, Matrix const& A, Matrix const& B) {
#pragma omp parallel for
  for (size_t i = 0; i < A.row(); i += 8) {
    for (size_t k = 0; k < A.col(); k++) {
      for (size_t j = 0; j < B.col(); j += 8) {
        block_avx2(C, A, B, i, j, k);
      }
    }
  }
}