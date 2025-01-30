#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <stdexcept>
#include <type_traits>

template <typename T> class Matrix {
  struct Row; // proxy for accessors

public: // raw constructors and destructor
  ~Matrix();
  Matrix(std::size_t, std::size_t);
  Matrix(const Matrix<T> &);

public: // static factory methods
  static Matrix<T> zero(std::size_t, std::size_t);
  static Matrix<T> zero_like(const Matrix<T> &);
  static Matrix<T> identity(std::size_t);
  static Matrix<T> identity_like(const Matrix<T> &);

public: // selectors
  const T *begin() const { return data; }
  const T *end() const { return data + ncols * nrows; }
  const Row operator[](std::size_t) const;

public: // modifiers
  T *begin() { return data; }
  T *end() { return data + ncols * nrows; }
  Row operator[](std::size_t);

public: // properties
  bool is_squared() const { return ncols == nrows; }

private: // fields and proxy
  std::size_t nrows = 0, ncols = 0;
  T *data = nullptr;

  struct Row {
    ~Row() = default;
    Row() = delete;
    Row(T *, std::size_t);
    T &operator[](std::size_t);
    const T &operator[](std::size_t) const;

  private:
    T *row_data;
    std::size_t ncols;
  };
};

// internal constructors & destructor
template <typename T> Matrix<T>::~Matrix() { delete[] data; }

template <typename T>
Matrix<T>::Matrix(std::size_t nrows, std::size_t ncols)
    : nrows(nrows), ncols(ncols), data(new T[ncols * nrows]{}) {}

template <typename T>
Matrix<T>::Matrix(const Matrix<T> &other) : Matrix(other.nrows, other.ncols) {
  std::copy(other.begin(), other.end(), data);
}

// static factory methods
template <typename T>
Matrix<T> Matrix<T>::zero(std::size_t nrows, std::size_t ncols) {
  static_assert(std::is_arithmetic<T>::value,
                "Zero() can be used only with arithmetical types");
  return Matrix<T>{nrows, ncols};
}

template <typename T> Matrix<T> Matrix<T>::zero_like(const Matrix<T> &source) {
  return Matrix<T>::zero(source.nrows, source.ncols);
}

template <typename T> Matrix<T> Matrix<T>::identity(std::size_t n) {
  static_assert(std::is_arithmetic<T>::value,
                "Identity() can be used only with arithmetical types");

  Matrix<T> identity = Matrix<T>::zero(n, n);
  for (auto i = 0; i < n; ++i) {
    identity.data[i * n + i] = 1;
  }
  return identity;
}

template <typename T>
Matrix<T> Matrix<T>::identity_like(const Matrix<T> &prototype) {
  if (!prototype.is_squared())
    throw std::invalid_argument("prototype matrix must be squared!\n");

  return Matrix<T>::identity(prototype.nrows);
}

// selectors and modifiers

template <typename T>
typename Matrix<T>::Row Matrix<T>::operator[](std::size_t row) {
  if (row >= this->nrows)
    throw std::out_of_range(
        "Row index is out of range: " + std::to_string(row) + "\n");

  return Row(data + row * ncols, ncols);
}

template <typename T>
const typename Matrix<T>::Row Matrix<T>::operator[](std::size_t row) const {
  if (row >= this->nrows)
    throw std::out_of_range(
        "Row index is out of range: " + std::to_string(row) + "\n");

  return Row(data + row * ncols, ncols);
}

template <typename T>
Matrix<T>::Row::Row(T *row_data, std::size_t ncols)
    : row_data(row_data), ncols(ncols) {}

template <typename T> T &Matrix<T>::Row::operator[](std::size_t col) {
  if (col >= ncols)
    throw std::out_of_range(
        "Column index is out of range: " + std::to_string(col) + "\n");

  return row_data[col];
}

template <typename T>
const T &Matrix<T>::Row::operator[](std::size_t col) const {
  if (col >= ncols)
    throw std::out_of_range(
        "Column index is out of range: " + std::to_string(col) + "\n");

  return row_data[col];
}
