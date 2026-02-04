#include "monomial.h"

#include <cassert>

namespace groebner::monomial {

Monomial::Monomial() : degree_(0), num_variables_(0) {}

Monomial::Monomial(size_t num_variables)
    : degree_(0), num_variables_(num_variables) {}

Monomial::Monomial(std::vector<size_t> exponents)
    : exponents_(std::move(exponents)), degree_(0) {
  num_variables_ = exponents_.size();
  for (size_t exp : exponents_) {
    degree_ += exp;
  }
}

size_t Monomial::GetDegree() const { return degree_; }

std::vector<size_t> Monomial::GetExponents() const { return exponents_; }

size_t Monomial::GetNumVariables() const { return num_variables_; }

bool Monomial::operator==(const Monomial& other) const {
  CheckCompatibility(other);
  return exponents_ == other.exponents_;
}

bool Monomial::operator<(const Monomial& other) const {
  CheckCompatibility(other);
  return exponents_ < other.exponents_;
}

Monomial Monomial::operator*(const Monomial& other) const {
  CheckCompatibility(other);
  assert(num_variables_ == other.num_variables_ &&
         "Incorrect monomial multiplying, monomials should have equal "
         "num_variables");
  Monomial res(*this);
  res.degree_ += other.degree_;

  for (size_t i = 0; i < num_variables_; ++i) {
    res.exponents_[i] += other.exponents_[i];
  }
  return res;
}

std::optional<Monomial> Monomial::CheckAndDivide(const Monomial& other) const {
  CheckCompatibility(other);
  Monomial res(*this);
  res.degree_ -= other.degree_;
  for (size_t i = 0; i < num_variables_; ++i) {
    if (res.exponents_[i] < other.exponents_[i]) {
      return std::nullopt;
    }
    res.exponents_[i] -= other.exponents_[i];
  }
  return res;
}

void Monomial::Print(const std::vector<std::string> names) const {
  for (size_t i = 0; i < exponents_.size(); ++i) {
    if (exponents_[i] != 0) {
      std::cout << names[i] << "^" << exponents_[i];
    }
  }
}

void Monomial::CheckCompatibility(const Monomial& other) const {
  assert(num_variables_ == other.num_variables_ &&
         "Monomials must have equal number of variables");
}

Monomial Monomial::LCM(const Monomial& other) const {
  CheckCompatibility(other);
  std::vector<size_t> result_exponents(exponents_.size());
  for (size_t i = 0; i < result_exponents.size(); ++i) {
    result_exponents[i] = std::max(exponents_[i], other.exponents_[i]);
  }
  return Monomial(result_exponents);
}

Monomial LCMMonomial(const Monomial& m1, const Monomial& m2) {
  return m1.LCM(m2);
}

}  // namespace groebner::monomial
