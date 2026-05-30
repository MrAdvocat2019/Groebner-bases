#pragma once
#include <map>

#include "monomial/monomial.h"
#include "orders.h"
#include "term.h"

namespace groebner {

template <typename F>
class Polynomial {
 public:
  Polynomial(Term<F> t, Order order) : terms_(std::move(order)) {
    *this += t;
  }

  explicit Polynomial(Order order) : terms_(std::move(order)) {}

  Order GetOrder() const { return terms_.key_comp(); }

  Polynomial& operator+=(const Term<F>& term) {
    auto& c = terms_[term.M()];
    c += term.Coeff();
    if (c == F(0)) {
      terms_.erase(term.M());
    }
    return *this;
  }

  Polynomial& operator+=(const Polynomial& other) {
    for (const auto& [monomial, coeff] : other.terms_) {
      terms_[monomial] += coeff;
    }
    RemoveZeroTerms();
    return *this;
  }

  Polynomial& operator-=(const Polynomial& other) {
    for (const auto& [monomial, coeff] : other.terms_) {
      terms_[monomial] -= coeff;
    }
    RemoveZeroTerms();
    return *this;
  }

  Polynomial& operator*=(const Polynomial& other) {
    Polynomial res(terms_.key_comp());
    for (const auto& [m1, c1] : terms_) {
      for (const auto& [m2, c2] : other.terms_) {
        res += Term<F>(m1 * m2, c1 * c2);
      }
    }
    *this = std::move(res);
    return *this;
  }

  bool operator==(const Polynomial& other) const {
    return terms_ == other.terms_;
  }
  bool operator!=(const Polynomial& other) const { return !(*this == other); }

  Term<F> LT() const {
    if (terms_.empty()) {
      return Term<F>(Monomial(), F(0));
    }
    auto it = terms_.rbegin();
    return Term<F>(it->first, it->second);
  }

  F LC() const {
    if (terms_.empty()) {
      return F(0);
    }
    return terms_.rbegin()->second;
  }

  Monomial LM() const {
    if (terms_.empty()) {
      return Monomial();
    }
    return terms_.rbegin()->first;
  }

  bool IsZero() const { return terms_.empty(); }

  const std::map<Monomial, F, Order>& Terms() const { return terms_; }

  friend Polynomial operator+(Polynomial left, const Polynomial& right) {
    left += right;
    return left;
  }
  friend Polynomial operator-(Polynomial left, const Polynomial& right) {
    left -= right;
    return left;
  }
  friend Polynomial operator*(Polynomial left, const Polynomial& right) {
    left *= right;
    return left;
  }

 private:
  void RemoveZeroTerms() {
    for (auto it = terms_.begin(); it != terms_.end();) {
      if (it->second == F(0)) {
        it = terms_.erase(it);
      } else {
        ++it;
      }
    }
  }

  std::map<Monomial, F, Order> terms_;
};

}  // namespace groebner
