#include <fmt/format.h>

#include <Eigen/Dense>
#include <ctime>
#include <iostream>

#include "timer.hpp"

using namespace Eigen;

int main() {
  // 矩阵大小
  int matrix_size = 1600;

  // 初始化矩阵 A 和 B，元素范围在 [0, 1] 之间
  MatrixXd A = MatrixXd::Random(matrix_size, matrix_size);
  MatrixXd B = MatrixXd::Random(matrix_size, matrix_size);

  // 初始化矩阵 C，全部元素设置为 0
  MatrixXd C = MatrixXd::Zero(matrix_size, matrix_size);

  // 执行矩阵乘法并记录执行时间
  tbs::Timer timer;
  auto t1 = std::chrono::steady_clock::now();
  C = A * B;
  auto t2 = std::chrono::steady_clock::now();
  fmt::print(
      "gemm_eigen: {} s\n",
      (double)std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1)
              .count() /
          1000000.0f);

  std::cout << C.block(0, 0, 2, 2) << std::endl;

  return 0;
}
