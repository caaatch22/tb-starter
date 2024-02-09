#ifndef GEMM_UTILS_H
#define GEMM_UTILS_H

#include <fmt/format.h>

#include <algorithm>
#include <ranges>

#include "random.hpp"
#include "timer.hpp"

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
  void random() { std::generate(data_, data_ + m_ * n_, tbs::rng_double); }

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
#pragma omp for simd
      for (size_t j = 0; j < col(); j++) {
        result(i, j) = (*this)(i, j);
      }
    }
    return result;
  }

  void from_aligned(Matrix const& src) {
    for (size_t i = 0; i < row(); i++) {
#pragma omp for simd
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

double GFlops(double sec, size_t m, size_t n, size_t k) {
  return 2.0 * m * n * k / sec / 1e9;
}

#endif  // GEMM_UTILS_H