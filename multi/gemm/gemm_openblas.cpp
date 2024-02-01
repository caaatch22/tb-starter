#include <cblas.h>

#include <ctime>
#include <iostream>

int main() {
  // 矩阵大小
  int matrix_size = 1600;

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
  clock_t start_time = clock();
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, matrix_size,
              matrix_size, matrix_size, 1.0, A, matrix_size, B, matrix_size,
              0.0, C, matrix_size);
  clock_t end_time = clock();

  // 计算矩阵乘法的执行时间
  double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

  // 打印执行时间和部分结果
  std::cout << "Matrix multiplication executed in " << execution_time
            << " seconds." << std::endl;
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
