#pragma once
#include <iostream>
#include <map>
#include <vector>

#include "orders.h"
#include "term.h"

namespace groebner {

template <typename Field>
class Polynomial {
 public:
  Polynomial(Term<Field> t, size_t num_ring_variables, Order order)
      : num_ring_variables_(num_ring_variables), terms_(std::move(order)) {
    *this += t;
  }

  Polynomial(size_t num_ring_variables, Order order)
      : num_ring_variables_(num_ring_variables), terms_(std::move(order)) {}

  size_t NumRingVariables() const { return num_ring_variables_; }
  Order GetOrder() const { return terms_.key_comp(); }

  Polynomial& operator+=(const Term<Field>& term) {
    auto& c = terms_[term.GetMonomial()];
    c += term.GetCoeff();
    if (c == Field(0)) {
      terms_.erase(term.GetMonomial());
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
    Polynomial res(num_ring_variables_, terms_.key_comp());
    for (const auto& [m1, c1] : terms_) {
      for (const auto& [m2, c2] : other.terms_) {
        res += Term<Field>(m1 * m2, c1 * c2);
      }
    }
    *this = std::move(res);
    return *this;
  }

  bool operator==(const Polynomial& other) const {
    return terms_ == other.terms_;
  }
  bool operator!=(const Polynomial& other) const { return !(*this == other); }

  Term<Field> LT() const {
    if (terms_.empty()) {
      return Term<Field>(Monomial(), Field(0));
    }
    auto it = terms_.rbegin();
    return Term<Field>(it->first, it->second);
  }

  Field LC() const {
    if (terms_.empty()) {
      return Field(0);
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

  void Print(const std::vector<std::string>& names) const {
    if (terms_.empty()) {
      std::cout << "0" << std::endl;
      return;
    }
    bool first = true;
    for (auto it = terms_.rbegin(); it != terms_.rend(); ++it) {
      const auto& [monomial, coeff] = *it;
      if (!first) {
        std::cout << " + ";
      }
      std::cout << coeff;
      monomial.Print(names);
      first = false;
    }
    std::cout << std::endl;
  }

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
      if (it->second == Field(0)) {
        it = terms_.erase(it);
      } else {
        ++it;
      }
    }
  }

  size_t num_ring_variables_;
  std::map<Monomial, Field, Order> terms_;
};

}  // namespace groebner
