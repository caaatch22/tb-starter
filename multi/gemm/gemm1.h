#pragma once

#include "gemm_utils.h"

void gemm1(Matrix& C, Matrix& A, Matrix& B) {
  for (size_t i = 0; i < A.row(); i += 4) {
    for (size_t j = 0; j < B.col(); j++) {
      for (size_t k = 0; k < B.row(); k++) {
        C(i, j) += A(i, k) * B(k, j);
        C(i + 1, j) += A(i + 1, k) * B(k, j);
        C(i + 2, j) += A(i + 2, k) * B(k, j);
        C(i + 3, j) += A(i + 3, k) * B(k, j);
      }
    }
  }
}

// void gemm1(Matrix& C, Matrix& A, Matrix& B) {
//   for (size_t i = 0; i < A.row(); i += 8) {
//     for (size_t j = 0; j < B.col(); j++) {
//       for (size_t k = 0; k < B.row(); k++) {
//         C(i, j) += A(i, k) * B(k, j);
//         C(i + 1, j) += A(i + 1, k) * B(k, j);
//         C(i + 2, j) += A(i + 2, k) * B(k, j);
//         C(i + 3, j) += A(i + 3, k) * B(k, j);
//         C(i + 4, j) += A(i + 4, k) * B(k, j);
//         C(i + 5, j) += A(i + 5, k) * B(k, j);
//         C(i + 6, j) += A(i + 6, k) * B(k, j);
//         C(i + 7, j) += A(i + 7, k) * B(k, j);
//       }
//     }
//   }
// }