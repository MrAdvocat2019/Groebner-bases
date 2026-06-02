#include "monomial.h"

#include <algorithm>
#include <cassert>

namespace groebner {

Monomial::Monomial(std::vector<size_t>&& clean_exponents)
    : exponents_(std::move(clean_exponents)),
      degree_(ComputeDegree(exponents_)) {
  assert(IsValid(exponents_, degree_) && "Monomial invariant violated");
}

Monomial Monomial::FromExponents(std::vector<size_t>&& exponents) {
  RemoveTrailingZeroes(&exponents);
  return Monomial(std::move(exponents));
}

Monomial Monomial::FromExponents(const std::vector<size_t>& exponents) {
  return FromExponents(std::vector<size_t>(exponents));
}

size_t Monomial::Degree() const { return degree_; }

const std::vector<size_t>& Monomial::Exponents() const { return exponents_; }

size_t Monomial::Size() const { return exponents_.size(); }

bool Monomial::operator==(const Monomial& other) const {
  return degree_ == other.degree_ && exponents_ == other.exponents_;
}

bool Monomial::operator!=(const Monomial& other) const {
  return !(*this == other);
}

Monomial& Monomial::operator*=(const Monomial& other) {
  if (exponents_.size() < other.exponents_.size()) {
    exponents_.resize(other.exponents_.size(), 0);
  }
  degree_ += other.degree_;
  for (size_t i = 0; i < other.exponents_.size(); ++i) {
    exponents_[i] += other.exponents_[i];
  }
  assert(IsValid(exponents_, degree_) && "Monomial invariant violated");
  return *this;
}

Monomial operator*(Monomial left, const Monomial& right) {
  left *= right;
  return left;
}

Monomial& Monomial::operator/=(const Monomial& other) {
  assert(IsDivisibleBy(other) && "Monomial is not divisible by other");
  for (size_t i = 0; i < other.exponents_.size(); ++i) {
    exponents_[i] -= other.exponents_[i];
  }
  RemoveTrailingZeroes(&exponents_);
  degree_ -= other.degree_;
  assert(IsValid(exponents_, degree_) && "Monomial invariant violated");
  return *this;
}

Monomial operator/(Monomial left, const Monomial& right) {
  left /= right;
  return left;
}

bool Monomial::IsDivisibleBy(const Monomial& other) const {
  if (other.exponents_.size() > exponents_.size()) {
    return false;
  }
  for (size_t i = 0; i < other.exponents_.size(); ++i) {
    if (exponents_[i] < other.exponents_[i]) {
      return false;
    }
  }
  return true;
}

Monomial Monomial::Lcm(const Monomial& other) const {
  size_t max_size = std::max(exponents_.size(), other.exponents_.size());
  std::vector<size_t> result_exponents(max_size);
  for (size_t i = 0; i < max_size; ++i) {
    size_t a = i < exponents_.size() ? exponents_[i] : 0;
    size_t b = i < other.exponents_.size() ? other.exponents_[i] : 0;
    result_exponents[i] = std::max(a, b);
  }
  RemoveTrailingZeroes(&result_exponents);
  return Monomial(std::move(result_exponents));
}

void Monomial::RemoveTrailingZeroes(std::vector<size_t>* v) {
  assert(v != nullptr);
  while (!v->empty() && v->back() == 0) {
    v->pop_back();
  }
}

size_t Monomial::ComputeDegree(const std::vector<size_t>& v) {
  size_t sum = 0;
  for (size_t exp : v) {
    sum += exp;
  }
  return sum;
}

bool Monomial::IsValid(const std::vector<size_t>& exponents, size_t degree) {
  if (!exponents.empty() && exponents.back() == 0) {
    return false;
  }
  return ComputeDegree(exponents) == degree;
}

}  // namespace groebner
