#pragma once

#include <omp.h>

#include <vector>

void gemm5(Matrix& C, Matrix const& A, Matrix const& B) {
  constexpr static size_t BLOCK_SIZE = 64;
  alignas(64) static float localA[BLOCK_SIZE][BLOCK_SIZE];
  alignas(64) static float localB[BLOCK_SIZE][BLOCK_SIZE];
  alignas(64) static float localC[BLOCK_SIZE][BLOCK_SIZE];

#pragma omp threadprivate(localA, localB, localC)
  size_t const matSize = A.row();
  size_t const blockNum = matSize / BLOCK_SIZE;
  fmt::print("blockNum: {}\n", blockNum);
// Traverse blocks.
#pragma omp parallel for
  for (size_t bi = 0; bi < blockNum; bi++) {
    for (size_t bj = 0; bj < blockNum; bj++) {
      // Clear localC.
      for (size_t i = 0; i < BLOCK_SIZE; i++) {
        for (size_t j = 0; j < BLOCK_SIZE; j += 8) {
          //   localC[i][j] = 0.0f;
          _mm256_storeu_ps(&localC[i][j], _mm256_setzero_ps());
        }
      }

      for (size_t bk = 0; bk < blockNum; bk++) {
        // Copy local block.
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
          for (size_t j = 0; j < BLOCK_SIZE; j++) {
            size_t aIdx = bi * BLOCK_SIZE * blockNum * BLOCK_SIZE +
                          i * blockNum * BLOCK_SIZE + bk * BLOCK_SIZE + j;
            size_t bIdx = bk * BLOCK_SIZE * blockNum * BLOCK_SIZE +
                          i * blockNum * BLOCK_SIZE + bj * BLOCK_SIZE + j;
            localA[i][j] = A(aIdx);
            localB[i][j] = B(bIdx);
          }
        }

        // Block GEMM.
        //         for (size_t i = 0; i < BLOCK_SIZE; i++) {
        //           for (size_t k = 0; k < BLOCK_SIZE; k++) {
        // #pragma omp simd
        //             for (size_t j = 0; j < BLOCK_SIZE; j++) {
        //               localC[i][j] += localA[i][k] * localB[k][j];
        //             }
        //           }
        //         }

        // Block GEMM.
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
          for (size_t k = 0; k < BLOCK_SIZE; k++) {
            // for (size_t j = 0; j < BLOCK_SIZE; j++) {
            //   localC[i][j] += localA[i][k] * localB[k][j];
            // }
            __m256 a = _mm256_set1_ps(localA[i][k]);
            for (size_t j = 0; j < BLOCK_SIZE; j += 8) {
              __m256 b = _mm256_loadu_ps(&localB[k][j]);
              __m256 c = _mm256_loadu_ps(&localC[i][j]);
              c = _mm256_fmadd_ps(a, b, c);
              _mm256_storeu_ps(&localC[i][j], c);
            }
          }
        }
      }
      // Copy localC back.
      for (size_t i = 0; i < BLOCK_SIZE; i++) {
#pragma omp simd
        for (size_t j = 0; j < BLOCK_SIZE; j++) {
          size_t cIdx = bi * BLOCK_SIZE * blockNum * BLOCK_SIZE +
                        i * blockNum * BLOCK_SIZE + bj * BLOCK_SIZE + j;
          C(cIdx) = localC[i][j];
          //   C(bi * BLOCK_SIZE * blockNum * BLOCK_SIZE,
          //     i * blockNum * BLOCK_SIZE + bj * BLOCK_SIZE + j) =
          //     localC[i][j];
        }
      }
    }
  }
}
