#include <omp.h>

#include <experimental/simd>

#include "gemm_utils.h"

namespace stdx = std::experimental;

void gemm6(Matrix& C, Matrix const& A, Matrix const& B) {
  static constexpr size_t SIMD_SIZE = stdx::native_simd<float>::size();
  static constexpr size_t BLOCK_SIZE = 64;
  fmt::print("SIMD_SIZE: {}, BLOCK_SIZE: {}\n", SIMD_SIZE, BLOCK_SIZE);
  alignas(64) static float local_a[BLOCK_SIZE][BLOCK_SIZE];
  alignas(64) static float local_b[BLOCK_SIZE][BLOCK_SIZE];
  alignas(64) static float local_c[BLOCK_SIZE][BLOCK_SIZE];

#pragma omp threadprivate(local_a, local_b, local_c)

  // auto aligned_c = C.make_aligned(BLOCK_SIZE);
  auto const aligned_a = A.make_aligned(BLOCK_SIZE);
  auto const aligned_b = B.make_aligned(BLOCK_SIZE);
  auto aligned_c = Matrix(aligned_a.row(), aligned_b.col());

  fmt::print("aligned_c.row(): {}, aligned_c.col(): {}\n", aligned_c.row(),
             aligned_c.col());

  size_t const ai_block_num = aligned_a.row() / BLOCK_SIZE;
  size_t const aj_block_num = aligned_b.col() / BLOCK_SIZE;
  size_t const bk_block_num = aligned_b.row() / BLOCK_SIZE;
  fmt::print("ai_block_num: {}, aj_block_num: {}, bk_block_num: {}\n",
             ai_block_num, aj_block_num, bk_block_num);

#pragma omp parallel for
  for (size_t bi = 0; bi < ai_block_num; bi++) {
    for (size_t bj = 0; bj < aj_block_num; bj++) {
      // Clear localC.
      for (size_t i = 0; i < BLOCK_SIZE; i++) {
#pragma omp simd
        // #pragma omp for simd
        // for (size_t j = 0; j < BLOCK_SIZE; j++) {
        //   local_c[i][j] = 0.0f;
        // }
        for (size_t j = 0; j < BLOCK_SIZE; j += SIMD_SIZE) {
          constexpr static stdx::native_simd<float> zero = 0.0f;
          zero.copy_to(&local_c[i][j], stdx::element_aligned);
          // _mm256_storeu_ps(&local_c[i][j], _mm256_setzero_ps());
        }
      }
      for (size_t bk = 0; bk < bk_block_num; bk++) {
        // Copy local block.
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
          for (size_t j = 0; j < BLOCK_SIZE; j++) {
            size_t const ax = bi * BLOCK_SIZE + i;
            size_t const ay = bk * BLOCK_SIZE + j;
            size_t const bx = bk * BLOCK_SIZE + i;
            size_t const by = bj * BLOCK_SIZE + j;
            // _mm256_storeu_ps(&local_a[i][j],
            //                  _mm256_loadu_ps(&aligned_a(ax, ay)));
            // _mm256_storeu_ps(&local_b[i][j],
            //                  _mm256_loadu_ps(&aligned_b(bx, by)));
            local_a[i][j] = aligned_a(ax, ay);
            local_b[i][j] = aligned_b(bx, by);
          }
        }

        // BLOCK_GEMM
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
          for (size_t k = 0; k < BLOCK_SIZE; k++) {
            // for (size_t j = 0; j < BLOCK_SIZE; j++) {
            //   local_c[i][j] += local_a[i][k] * local_b[k][j];
            // }
            stdx::native_simd<float> a = local_a[i][k];
            // __m256 a = _mm256_set1_ps(local_a[i][k]);
            for (size_t j = 0; j < BLOCK_SIZE; j += SIMD_SIZE) {
              stdx::native_simd<float> b;
              b.copy_from(&local_b[k][j], stdx::element_aligned);
              stdx::native_simd<float> c;
              c.copy_from(&local_c[i][j], stdx::element_aligned);
              c += a * b;
              c.copy_to(&local_c[i][j], stdx::element_aligned);
              // __m256 b = _mm256_loadu_ps(&local_b[k][j]);
              // __m256 c = _mm256_loadu_ps(&local_c[i][j]);
              // c = _mm256_fmadd_ps(a, b, c);
              // _mm256_storeu_ps(&local_c[i][j], c);
            }
          }
        }
      }

      for (size_t i = 0; i < BLOCK_SIZE; i++) {
        // #pragma omp simd

        for (size_t j = 0; j < BLOCK_SIZE; j += SIMD_SIZE) {
          size_t const cx = bi * BLOCK_SIZE + i;
          size_t const cy = bj * BLOCK_SIZE + j;
          stdx::native_simd<float> c;
          c.copy_from(&local_c[i][j], stdx::element_aligned);
          c.copy_to(&aligned_c(cx, cy), stdx::element_aligned);
          // _mm256_storeu_ps(&aligned_c(cx, cy),
          // _mm256_loadu_ps(&local_c[i][j])); aligned_c(cx, cy) =
          // local_c[i][j];
        }
      }
    }
  }

  C.from_aligned(aligned_c);
}