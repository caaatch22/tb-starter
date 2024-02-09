#pragma once
#include "gemm_utils.h"

// constexpr size_t BLOCK_SIZE = 64;
void gemm5_1(Matrix& C, Matrix const& A, Matrix const& B) {
  size_t const matSize = A.row();
  //   void gemmBlockCopy2(const std::vector<float>& matA,
  //                       const std::vector<float>& matB, std::vector<float>&
  //                       matC, size_t matSize) {
  size_t const blockNum = matSize / BLOCK_SIZE;

// Traverse blocks.
#pragma omp parallel for
  for (size_t bi = 0; bi < blockNum; bi++) {
    for (size_t bj = 0; bj < blockNum; bj++) {
      for (size_t bk = 0; bk < blockNum; bk++) {
        // Block GEMM.
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
          for (size_t k = 0; k < BLOCK_SIZE; k++) {
            float sum = 0.0;
            for (size_t j = 0; j < BLOCK_SIZE; j++) {
              sum += A(bk * BLOCK_SIZE + i, bj * BLOCK_SIZE + j) *
                     B(bi * BLOCK_SIZE + i, bk * BLOCK_SIZE + j);
              //   size_t aIdx = bi * BLOCK_SIZE * blockNum * BLOCK_SIZE +
              //                 i * blockNum * BLOCK_SIZE + bk * BLOCK_SIZE +
              //                 j;
              //   size_t bIdx = bk * BLOCK_SIZE * blockNum * BLOCK_SIZE +
              //                 i * blockNum * BLOCK_SIZE + bj * BLOCK_SIZE +
              //                 j;
              //   size_t cIdx = bi * BLOCK_SIZE * blockNum * BLOCK_SIZE +
              //                 i * blockNum * BLOCK_SIZE + bj * BLOCK_SIZE +
              //                 j;
              //   C(bi * BLOCK_SIZE + i, bj * BLOCK_SIZE + j) +=
              //       A(bk * BLOCK_SIZE + i, bj * BLOCK_SIZE + j) *
              //       B(bi * BLOCK_SIZE + i, bk * BLOCK_SIZE + j);
              //   C(cIdx) += A(aIdx) * B(bIdx);
              //   localC[i][j] += localA[i][k] * localB[k][j];
            }
            C(bi * BLOCK_SIZE + i, bj * BLOCK_SIZE + k) += sum;
            // __m256 a = _mm256_set1_ps(localA[i][k]);
            // for (size_t j = 0; j < BLOCK_SIZE; j += 8) {
            //   __m256 b = _mm256_loadu_ps(&localB[k][j]);
            //   __m256 c = _mm256_loadu_ps(&localC[i][j]);
            //   c = _mm256_fmadd_ps(a, b, c);
            //   _mm256_storeu_ps(&localC[i][j], c);
            //   }
          }
        }
      }
    }
  }
}
