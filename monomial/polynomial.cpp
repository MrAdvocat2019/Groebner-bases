#include "polynomial.h"

#include <cmath>
#include <iostream>

namespace groebner {
namespace polynomial {

Polynomial::Polynomial(Term t, OrderVariant strategy)
    : terms_(std::move(strategy)), num_variables_(t.m.GetNumVariables()) {
  AddTerm(t);
}

Polynomial::Polynomial(size_t num_variables, OrderVariant strategy)
    : terms_(std::move(strategy)), num_variables_(num_variables) {}

size_t Polynomial::GetNumVariables() const { return num_variables_; }

OrderVariant Polynomial::GetStrategy() const {
  return terms_.key_comp().GetStrategy();
}

void Polynomial::AddTerm(Term term) { terms_[std::move(term.m)] += term.coeff; }

Polynomial Polynomial::operator+(const Polynomial& other) {
  CheckCompatibility(other);
  Polynomial res(*this);
  for (const auto& [monomial, coeff] : other.terms_) {
    res.terms_[monomial] += coeff;
  }
  res.CleanUp();
  return res;
}

Polynomial Polynomial::operator-(const Polynomial& other) {
  CheckCompatibility(other);
  Polynomial res(*this);
  for (const auto& [monomial, coeff] : other.terms_) {
    res.terms_[monomial] -= coeff;
  }
  res.CleanUp();
  return res;
}

Polynomial Polynomial::operator*(const Polynomial& other) {
  CheckCompatibility(other);
  Polynomial res(num_variables_, terms_.key_comp().GetStrategy());
  for (auto& [monomial1, coeff1] : terms_) {
    for (auto& [monomial2, coeff2] : other.terms_) {
      res.AddTerm(Term(monomial1 * monomial2, coeff1 * coeff2));
    }
  }
  res.CleanUp();
  return res;
}

Polynomial& Polynomial::operator+=(const Polynomial& other) {
  CheckCompatibility(other);
  for (const auto& [monomial, coeff] : other.terms_) {
    terms_[monomial] += coeff;
  }
  CleanUp();
  return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& other) {
  CheckCompatibility(other);
  for (const auto& [monomial, coeff] : other.terms_) {
    terms_[monomial] -= coeff;
  }
  CleanUp();
  return *this;
}

Term Polynomial::LT() const {
  for (auto it = terms_.rbegin(); it != terms_.rend(); ++it) {
    if (std::abs(it->second) > 1e-9) {
      return Term(it->first, it->second);
    }
  }
  return Term(Monomial(num_variables_), 0);
}

double Polynomial::LC() const {
  for (auto it = terms_.rbegin(); it != terms_.rend(); ++it) {
    if (std::abs(it->second) > 1e-9) {
      return it->second;
    }
  }
  return 0;
}

Monomial Polynomial::LM() const {
  for (auto it = terms_.rbegin(); it != terms_.rend(); ++it) {
    if (std::abs(it->second) > 1e-9) {
      return it->first;
    }
  }
  return Monomial(num_variables_);
}

bool Polynomial::IsZero() const {
  if (terms_.empty()) return true;
  for (const auto& [m, c] : terms_) {
    if (std::abs(c) > 1e-9) return false;
  }
  return true;
}

void Polynomial::Print(const std::vector<std::string>& names) const {
  if (terms_.empty()) {
    std::cout << "0" << std::endl;
    return;
  }
  bool first = true;
  for (auto it = terms_.rbegin(); it != terms_.rend(); ++it) {
    const auto& [monomial, coeff] = *it;
    if (coeff != 0) {
      if (!first) std::cout << " + ";
      std::cout << coeff;
      monomial.Print(names);
      first = false;
    }
  }
  std::cout << std::endl;
}

void Polynomial::CheckCompatibility(const Polynomial& other) const {
  assert(num_variables_ == other.num_variables_ &&
         "Num variables is incompeteble");
  assert(terms_.key_comp() == other.terms_.key_comp() &&
         "Not equal monomial ordering");
}

void Polynomial::CleanUp() {
  for (auto it = terms_.begin(); it != terms_.end();) {
    if (std::abs(it->second) < 1e-9) {
      it = terms_.erase(it);
    } else {
      ++it;
    }
  }
}

}  // namespace polynomial
}  // namespace groebner
