#pragma once

#include "gemm_utils.h"

inline void block(Matrix& C, Matrix& A, Matrix& B, size_t i, size_t j,
                  size_t k) {
  for (size_t ii = i; ii < i + 4; ii++) {
    for (size_t jj = j; jj < j + 4; jj++) {
      C(ii, jj) += A(ii, k) * B(k, jj);
    }
  }
}

void gemm2(Matrix& C, Matrix& A, Matrix& B) {
  for (size_t i = 0; i < A.row(); i += 4) {
    for (size_t j = 0; j < B.col(); j += 4) {
      for (size_t k = 0; k < B.row(); k++) {
        block(C, A, B, i, j, k);

        // C(i, j) += A(i, k) * B(k, j);
        // C(i + 1, j) += A(i + 1, k) * B(k, j);
        // C(i + 2, j) += A(i + 2, k) * B(k, j);
        // C(i + 3, j) += A(i + 3, k) * B(k, j);

        // C(i, j + 1) += A(i, k) * B(k, j + 1);
        // C(i + 1, j + 1) += A(i + 1, k) * B(k, j + 1);
        // C(i + 2, j + 1) += A(i + 2, k) * B(k, j + 1);
        // C(i + 3, j + 1) += A(i + 3, k) * B(k, j + 1);

        // C(i, j + 2) += A(i, k) * B(k, j + 2);
        // C(i + 1, j + 2) += A(i + 1, k) * B(k, j + 2);
        // C(i + 2, j + 2) += A(i + 2, k) * B(k, j + 2);
        // C(i + 3, j + 2) += A(i + 3, k) * B(k, j + 2);

        // C(i, j + 3) += A(i, k) * B(k, j + 3);
        // C(i + 1, j + 3) += A(i + 1, k) * B(k, j + 3);
        // C(i + 2, j + 3) += A(i + 2, k) * B(k, j + 3);
        // C(i + 3, j + 3) += A(i + 3, k) * B(k, j + 3);
      }
    }
  }
}