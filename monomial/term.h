#pragma once

#include <optional>

#include "monomial.h"

namespace groebner {

template <typename Field>
class Term {
 public:
  Term(const Monomial& m, Field coeff) : m_(m), coeff_(std::move(coeff)) {}

  const Field& GetCoeff() const { return coeff_; }
  const Monomial& GetMonomial() const { return m_; }

  bool operator==(const Term& other) const {
    return m_ == other.m_ && coeff_ == other.coeff_;
  }
  bool operator!=(const Term& other) const { return !(*this == other); }

  Term& operator*=(const Term& other) {
    m_ *= other.m_;
    coeff_ *= other.coeff_;
    return *this;
  }

  std::optional<Term> DivideBy(const Term& other) const {
    auto result_monomial = m_.DivideBy(other.m_);
    if (!result_monomial.has_value()) {
      return std::nullopt;
    }
    return Term(result_monomial.value(), coeff_ / other.coeff_);
  }

  Term Lcm(const Term& other) const {
    return Term(m_.Lcm(other.m_), coeff_ * other.coeff_);
  }

  friend Term operator*(Term left, const Term& right) {
    left *= right;
    return left;
  }

 private:
  Monomial m_;
  Field coeff_;
};

}  // namespace groebner
