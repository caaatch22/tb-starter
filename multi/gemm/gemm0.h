#pragma once

#include "gemm_utils.h"

void gemm0(Matrix& C, Matrix& A, Matrix& B) {
  // #pragma omp parallel for
  for (size_t i = 0; i < A.row(); i++) {
    for (size_t j = 0; j < B.col(); j++) {
      for (size_t k = 0; k < A.col(); k++) {
        C(i, j) += A(i, k) * B(k, j);
      }
    }
  }
}
