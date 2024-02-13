#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <immintrin.h>
#include <cublas_v2.h>

class Timer {
public:
 Timer() noexcept : start(std::chrono::high_resolution_clock::now()) {}

 [[nodiscard]] auto elapsed() const noexcept -> std::chrono::duration<double> {
   const auto now = std::chrono::high_resolution_clock::now();
   return std::chrono::duration<double>(now - start);
 }

 void reset() noexcept { start = std::chrono::high_resolution_clock::now(); }

private:
 std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

static std::random_device seed_random_device;
static std::mt19937 engine(seed_random_device());
static std::mt19937_64 engine64(seed_random_device());

inline static double rng_double() {
  return static_cast<double>(engine()) / engine.max();
}

class Matrix {
public:
 Matrix(size_t m, size_t n)
     : m_(m), n_(n), data_(new(std::align_val_t(64)) float[m * n]) {}
 ~Matrix() { delete[] data_; }

 Matrix(Matrix const& rhs)
     : m_(rhs.m_), n_(rhs.n_), data_(new float[m_ * n_]) {
   std::copy(rhs.data_, rhs.data_ + m_ * n_, data_);
 }
 Matrix(Matrix&& rhs) noexcept : m_(rhs.m_), n_(rhs.n_), data_(rhs.data_) {}
 [[nodiscard]] bool operator==(const Matrix& rhs) const noexcept {
   return m_ == rhs.m_ && n_ == rhs.n_ &&
          std::equal(data_, data_ + m_ * n_, rhs.data_,
                     [](float a, float b) { return std::abs(a - b) < 1e-6; });
 }

 [[nodiscard]] bool operator!=(const Matrix& rhs) const noexcept { return !(*this == rhs); }

 Matrix& operator=(Matrix const& rhs) {
   if (this != &rhs) {
     Matrix tmp(rhs);
     std::swap(m_, tmp.m_);
     std::swap(n_, tmp.n_);
     std::swap(data_, tmp.data_);
   }
   return *this;
 }

 Matrix& operator=(Matrix&& rhs) {
   std::swap(m_, rhs.m_);
   std::swap(n_, rhs.n_);
   std::swap(data_, rhs.data_);
   return *this;
 }


 [[nodiscard]] float const* data() const noexcept { return data_; }
 [[nodiscard]] float* data() noexcept { return data_; }
 [[nodiscard]] size_t row() const noexcept { return m_; }
 [[nodiscard]] size_t col() const noexcept { return n_; }

 void zeros() {
   // #pragma omp simd
   for (size_t i = 0; i < m_ * n_; i++) {
     data_[i] = 0.0f;
   }
 }
 void ones() { std::fill(data_, data_ + m_ * n_, 1.0f); }
 void random() { std::generate(data_, data_ + m_ * n_, rng_double); }

 [[nodiscard]] float& operator()(size_t i, size_t j) {
   return data_[i * n_ + j];
 }

 [[nodiscard]] float const& operator()(size_t i, size_t j) const {
   return data_[i * n_ + j];
 }

 [[nodiscard]] float& operator()(size_t i) { return data_[i]; }
 [[nodiscard]] float const& operator()(size_t i) const { return data_[i]; }

 Matrix make_aligned(size_t const block_size) const {
   auto const rows = ((row() + block_size - 1) / block_size) * block_size;
   auto const cols = ((col() + block_size - 1) / block_size) * block_size;
   Matrix result(rows, cols);
   for (size_t i = 0; i < row(); i++) {
     for (size_t j = 0; j < col(); j++) {
       result(i, j) = (*this)(i, j);
     }
   }
   return result;
 }

 void from_aligned(Matrix const& src) {
   for (size_t i = 0; i < row(); i++) {
     for (size_t j = 0; j < col(); j++) {
       (*this)(i, j) = src(i, j);
     }
   }
 }

private:
 size_t m_;
 size_t n_;
 alignas(64) float* data_;
};

void gemm(Matrix& C, Matrix const& A, Matrix const& B) {
  constexpr static size_t BLOCK_SIZE = 64;
  alignas(64) static float localA[BLOCK_SIZE][BLOCK_SIZE];
  alignas(64) static float localB[BLOCK_SIZE][BLOCK_SIZE];
  alignas(64) static float localC[BLOCK_SIZE][BLOCK_SIZE];
  
#pragma omp threadprivate(localA, localB, localC)
  size_t const matSize = A.row();
  size_t const blockNum = matSize / BLOCK_SIZE;
    // Traverse blocks.
#pragma omp parallel for
    for (size_t bi = 0; bi < blockNum; bi++) {
      for (size_t bj = 0; bj < blockNum; bj++) {
        // Clear localC.
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
#pragma omp simd
          for (size_t j = 0; j < BLOCK_SIZE; j ++) {
              localC[i][j] = 0.0f;
          }
        }
  
        for (size_t bk = 0; bk < blockNum; bk++) {
          // Copy local block.
          for (size_t i = 0; i < BLOCK_SIZE; i++) {
            for (size_t j = 0; j < BLOCK_SIZE; j++) {
              size_t const aIdx = bi * BLOCK_SIZE * blockNum * BLOCK_SIZE +
                            i * blockNum * BLOCK_SIZE + bk * BLOCK_SIZE + j;
              size_t const bIdx = bk * BLOCK_SIZE * blockNum * BLOCK_SIZE +
                            i * blockNum * BLOCK_SIZE + bj * BLOCK_SIZE + j;
              localA[i][j] = A(aIdx);
              localB[i][j] = B(bIdx);
            }
          }
  
        //   Block GEMM.
                  for (size_t i = 0; i < BLOCK_SIZE; i++) {
                    for (size_t k = 0; k < BLOCK_SIZE; k++) {
          #pragma omp simd
                      for (size_t j = 0; j < BLOCK_SIZE; j++) {
                        localC[i][j] += localA[i][k] * localB[k][j];
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
          }
        }
      }
    }
  }
  

double GFlops(size_t m, size_t n, size_t k, double seconds) {
  return 2.0 * m * n * k / (1e9 * seconds);
}

#define TILE_WIDTH 64
__global__ void matrixMulKernel(float* M, float* N, float* P, int Width) {
    __shared__ float Mds[TILE_WIDTH][TILE_WIDTH];
    __shared__ float Nds[TILE_WIDTH][TILE_WIDTH];

    int bx = blockIdx.x, by = blockIdx.y;
    int tx = threadIdx.x, ty = threadIdx.y;

    int row = by * TILE_WIDTH + ty;
    int col = bx * TILE_WIDTH + tx;

    float value = 0.0f;
    for (size_t i = 0; i < Width / TILE_WIDTH; i ++) {
        Mds[ty][tx] = M[row * Width + i * TILE_WIDTH + tx];
        Nds[ty][tx] = M[(i * TILE_WIDTH + ty) * Width + col];
        __syncthreads();

        for (size_t k = 0; k < TILE_WIDTH; k ++) {
            value += Mds[ty][k] * Nds[k][tx];
        }
        __syncthreads();
    }
    P[row * Width + col] = value;
}

void check_equal(Matrix const& A, Matrix const& B) {
    if (A != B) {
      for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            std::cout << A(i, j) << " ";
            std::cout << B(i, j) << std::endl;
        }
      }
      throw std::runtime_error("A != B");
    }
  }

int main() {
  constexpr int N = 1024 * 8;
  Matrix A(N, N);
  Matrix B(N, N);
  Matrix C(N, N);
  A.random();
  B.random();
  Timer timer;
  gemm(C, A, B);
  auto time = timer.elapsed();

  std::cout << "Time: " << time.count() << " s" << std::endl;
  std::cout << "GFlops: " << GFlops(N, N, N, time.count()) << std::endl;
  for (size_t i = 0; i < 2; i++) {
    for (size_t j = 0; j < 2; j++) {
      std::cout << C(i, j) << " ";
    }
    std::cout << std::endl;
  }

  Matrix C2(N, N);

  float* d_A, *d_B, *d_C;
  cudaMalloc(&d_A, N * N * sizeof(float));
  cudaMalloc(&d_B, N * N * sizeof(float));
  cudaMalloc(&d_C, N * N * sizeof(float));

  timer.reset();
  cudaMemcpy(d_A, A.data(), N * N * sizeof(float), cudaMemcpyHostToDevice);
  cudaMemcpy(d_B, B.data(), N * N * sizeof(float), cudaMemcpyHostToDevice);
  dim3 dimGrid(N / TILE_WIDTH, N / TILE_WIDTH, 1);
  dim3 dimBlock(TILE_WIDTH, TILE_WIDTH, 1);
  matrixMulKernel<<<dimGrid, dimBlock>>>(d_A, d_B, d_C, N);
  cudaMemcpy(C2.data(), d_C, N * N * sizeof(float), cudaMemcpyDeviceToHost);
  time = timer.elapsed();
  std::cout << "Time: " << time.count() << " s" << std::endl;
  std::cout << "GFlops: " << GFlops(N, N, N, time.count()) << std::endl;
//   check_equal(C, C2);
  return 0;
}