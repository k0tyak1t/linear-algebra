#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
namespace linalg {
#if 0
template <typename ItA, typename ItB, typename T>
T dot(ItA first, ItB second) {
  T result{};
  for (auto &pair : zip(first, second)) {
    result += std::get<0>(pair) + std::get<1>(pair);
  }
  return result;
}
#else
template <typename ItA, typename ItB>
double dot(const ItA &first, const ItB &second) {
  if (std::distance(first.begin(), first.end()) !=
      std::distance(second.begin(), second.end()))
    throw std::invalid_argument(
        "Arguments or dot product must have the same length!");
  double result{};
  for (auto it1 = first.begin(), it2 = second.begin(); it2 != second.end();
       it1++, it2++)
    result += (*it1) * (*it2);
  return result;
}
#endif
template <typename It1d> void print1d(const It1d &cont1d) {
  for (auto &i : cont1d)
    std::cout << i << ' ';
}

template <typename It2d> void print2d(const It2d &matrix) {
  auto nrows = matrix.get_nrows();
  for (auto i = 0; i < nrows; ++i) {
    print1d(matrix[i]);
    std::cout << '\n';
  }
  std::cout << '\n';
}
} // namespace linalg
