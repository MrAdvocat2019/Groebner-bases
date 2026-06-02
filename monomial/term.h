#pragma once

#include "monomial.h"

namespace groebner {

template <typename F>
class Term {
 public:
  Term(const Monomial& m, F coeff) : m_(m), coeff_(std::move(coeff)) {}

  const F& Coeff() const { return coeff_; }
  const Monomial& M() const { return m_; }

  bool operator==(const Term& other) const {
    return m_ == other.m_ && coeff_ == other.coeff_;
  }
  bool operator!=(const Term& other) const { return !(*this == other); }

  Term& operator*=(const Term& other) {
    m_ *= other.m_;
    coeff_ *= other.coeff_;
    return *this;
  }

  bool IsDivisibleBy(const Term& other) const {
    return m_.IsDivisibleBy(other.m_) && other.coeff_ != 0;
  }

  Term& operator/=(const Term& other) {
    m_ /= other.m_;
    coeff_ /= other.coeff_;
    return *this;
  }

  Term Lcm(const Term& other) const { return Term(m_.Lcm(other.m_), 1); }

  friend Term operator*(Term left, const Term& right) {
    left *= right;
    return left;
  }

  friend Term operator/(Term left, const Term& right) {
    left /= right;
    return left;
  }

 private:
  Monomial m_;
  F coeff_;
};

}  // namespace groebner
