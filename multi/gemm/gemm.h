#pragma once

#include <omp.h>

#include <experimental/simd>

#include "gemm_utils.h"

void gemm(Matrix& C, Matrix const& A, Matrix const& B) {
  namespace stdx = std::experimental;

  static constexpr size_t SIMD_SIZE = stdx::native_simd<float>::size();
  static constexpr size_t BLOCK_SIZE = 64;
  alignas(64) static float local_a[BLOCK_SIZE][BLOCK_SIZE];
  alignas(64) static float local_b[BLOCK_SIZE][BLOCK_SIZE];
  alignas(64) static float local_c[BLOCK_SIZE][BLOCK_SIZE];

#pragma omp threadprivate(local_a, local_b, local_c)

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
        for (size_t j = 0; j < BLOCK_SIZE; j += SIMD_SIZE) {
          constexpr static stdx::native_simd<float> zero = 0.0f;
          zero.copy_to(&local_c[i][j], stdx::element_aligned);
        }
      }
      for (size_t bk = 0; bk < bk_block_num; bk++) {
        // Copy local block.
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
          for (size_t j = 0; j < BLOCK_SIZE; j += SIMD_SIZE) {
            size_t const ax = bi * BLOCK_SIZE + i;
            size_t const ay = bk * BLOCK_SIZE + j;
            size_t const bx = bk * BLOCK_SIZE + i;
            size_t const by = bj * BLOCK_SIZE + j;
            stdx::native_simd<float> a(&aligned_a(ax, ay),
                                       stdx::element_aligned);
            stdx::native_simd<float> b(&aligned_b(bx, by),
                                       stdx::element_aligned);
            a.copy_to(&local_a[i][j], stdx::element_aligned);
            b.copy_to(&local_b[i][j], stdx::element_aligned);
          }
        }

        // BLOCK_GEMM
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
          for (size_t k = 0; k < BLOCK_SIZE; k++) {
            stdx::native_simd<float> a = local_a[i][k];
            for (size_t j = 0; j < BLOCK_SIZE; j += SIMD_SIZE) {
              stdx::native_simd<float> b;
              stdx::native_simd<float> c;
              b.copy_from(&local_b[k][j], stdx::element_aligned);
              c.copy_from(&local_c[i][j], stdx::element_aligned);
              c += a * b;
              c.copy_to(&local_c[i][j], stdx::element_aligned);
            }
          }
        }
      }

      for (size_t i = 0; i < BLOCK_SIZE; i++) {
        for (size_t j = 0; j < BLOCK_SIZE; j += SIMD_SIZE) {
          size_t const cx = bi * BLOCK_SIZE + i;
          size_t const cy = bj * BLOCK_SIZE + j;
          stdx::native_simd<float> c;
          c.copy_from(&local_c[i][j], stdx::element_aligned);
          c.copy_to(&aligned_c(cx, cy), stdx::element_aligned);
        }
      }
    }
  }

  C.from_aligned(aligned_c);
}