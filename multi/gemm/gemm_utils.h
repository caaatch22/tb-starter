#ifndef GEMM_UTILS_H
#define GEMM_UTILS_H

#include <fmt/format.h>

#include <algorithm>
#include <ranges>

#include "random.hpp"
#include "timer.hpp"

class Matrix {
 public:
  Matrix(size_t m, size_t n) : m_(m), n_(n), data_(new double[m * n]) {}
  ~Matrix() { delete[] data_; }

  Matrix(const Matrix& rhs)
      : m_(rhs.m_), n_(rhs.n_), data_(new double[m_ * n_]) {
    std::copy(rhs.data_, rhs.data_ + m_ * n_, data_);
  }
  Matrix(Matrix&& rhs) noexcept : m_(rhs.m_), n_(rhs.n_), data_(rhs.data_) {}
  [[nodiscard]] bool operator==(const Matrix& rhs) const noexcept {
    return m_ == rhs.m_ && n_ == rhs.n_ &&
           std::equal(
               data_, data_ + m_ * n_, rhs.data_,
               [](double a, double b) { return std::abs(a - b) < 1e-6; });
  }

  [[nodiscard]] double* data() noexcept { return data_; }
  [[nodiscard]] size_t row() const noexcept { return m_; }
  [[nodiscard]] size_t col() const noexcept { return n_; }

  void zeros() { std::fill(data_, data_ + m_ * n_, 0.0f); }
  void ones() { std::fill(data_, data_ + m_ * n_, 1.0f); }
  void random() { std::generate(data_, data_ + m_ * n_, tbs::rng_double); }

  [[nodiscard]] double& operator()(size_t i, size_t j) {
    return data_[i * n_ + j];
  }

 private:
  size_t m_;
  size_t n_;
  double* data_;
};

#endif  // GEMM_UTILS_H