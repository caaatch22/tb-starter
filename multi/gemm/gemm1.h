#pragma once

#include "gemm_utils.h"

// loop interchange, order: i, k, j
void gemm1(Matrix& C, Matrix const& A, Matrix const& B) {
  for (size_t i = 0; i < A.row(); i++) {
    for (size_t k = 0; k < A.col(); k++) {
      for (size_t j = 0; j < B.col(); j++) {
        C(i, j) += A(i, k) * B(k, j);
      }
    }
  }
}