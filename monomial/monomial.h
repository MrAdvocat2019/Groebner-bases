#pragma once
#include <optional>
#include <vector>

namespace groebner {

class Monomial {
 public:
  Monomial() = default;
  static Monomial FromExponents(std::vector<size_t>&& exponents);
  static Monomial FromExponents(const std::vector<size_t>& exponents);

  size_t Degree() const;
  const std::vector<size_t>& Exponents() const;
  size_t Size() const;

  bool operator==(const Monomial& other) const;
  bool operator!=(const Monomial& other) const;

  Monomial& operator*=(const Monomial& other);
  Monomial& operator/=(const Monomial& other);
  bool IsDivisibleBy(const Monomial& other) const;
  std::optional<Monomial> DivideBy(const Monomial& other) const;

  Monomial Lcm(const Monomial& other) const;

  friend Monomial operator*(Monomial left, const Monomial& right);
  friend Monomial operator/(Monomial left, const Monomial& right);

 private:
  explicit Monomial(std::vector<size_t>&& clean_exponents);

  static void RemoveTrailingZeroes(std::vector<size_t>* v);
  static size_t ComputeDegree(const std::vector<size_t>& v);
  static bool IsValid(const std::vector<size_t>& exponents, size_t degree);

  std::vector<size_t> exponents_;
  size_t degree_ = 0;
};

}  // namespace groebner
