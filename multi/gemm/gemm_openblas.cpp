#include <cblas.h>

#include <chrono>
#include <ctime>
#include <iostream>

int main() {
  // 矩阵大小
  int matrix_size = 10000;

  // 初始化矩阵 A 和 B，元素范围在 [0, 1] 之间
  double* A = new double[matrix_size * matrix_size];
  double* B = new double[matrix_size * matrix_size];
  for (int i = 0; i < matrix_size * matrix_size; i++) {
    A[i] = (double)rand() / RAND_MAX;
    B[i] = (double)rand() / RAND_MAX;
  }

  // 初始化矩阵 C，全部元素设置为 0
  double* C = new double[matrix_size * matrix_size];
  for (int i = 0; i < matrix_size * matrix_size; i++) {
    C[i] = 0.0;
  }

  // 执行矩阵乘法并记录执行时间
  auto start = std::chrono::high_resolution_clock::now();
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, matrix_size,
              matrix_size, matrix_size, 1.0, A, matrix_size, B, matrix_size,
              0.0, C, matrix_size);
  auto end = std::chrono::high_resolution_clock::now();
  // 计算矩阵乘法的执行时间
  double execution_time =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count() /
      1000.0;

  // 打印执行时间和部分结果
  std::cout << "gemm_openblas: " << execution_time
            << " s, Glops: " << std::fixed
            << (2.0 * matrix_size * matrix_size * matrix_size / execution_time /
                1e9)
            << std::endl;
  std::cout << "Partial result:" << std::endl;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      std::cout << C[i * matrix_size + j] << " ";
    }
    std::cout << std::endl;
  }

  // 释放内存
  delete[] A;
  delete[] B;
  delete[] C;

  return 0;
}
