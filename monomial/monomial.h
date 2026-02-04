#pragma once
#include <cassert>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

namespace groebner::monomial {

class Monomial {
 public:
  Monomial();
  explicit Monomial(size_t num_variables);
  explicit Monomial(std::vector<size_t> exponents);

  size_t GetDegree() const;
  std::vector<size_t> GetExponents() const;
  size_t GetNumVariables() const;

  bool operator==(const Monomial& other) const;
  bool operator<(const Monomial& other) const;

  Monomial operator*(const Monomial& other) const;
  std::optional<Monomial> CheckAndDivide(const Monomial& other) const;

  Monomial LCM(const Monomial& other) const;

  void Print(const std::vector<std::string> names) const;

 private:
  size_t degree_;
  size_t num_variables_;
  std::vector<size_t> exponents_;

  void CheckCompatibility(const Monomial& other) const;
};

Monomial LCMMonomial(const Monomial& m1, const Monomial& m2);

}  // namespace groebner::monomial