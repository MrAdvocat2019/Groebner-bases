#pragma once
#include <map>

#include "comparator.h"
#include "term.h"

namespace groebner::polynomial {

using monomial::Monomial;
using monomial::Term;
using ordering::LexOrder;
using ordering::OrderVariant;
using ordering::PolynomialOrdering;

class Polynomial {
 public:
  Polynomial(Term t, OrderVariant strategy);
  Polynomial(size_t num_variables, OrderVariant strategy = LexOrder{});

  size_t GetNumVariables() const;
  OrderVariant GetStrategy() const;

  void AddTerm(Term term);

  Polynomial operator+(const Polynomial& other);
  Polynomial operator-(const Polynomial& other);
  Polynomial operator*(const Polynomial& other);

  Polynomial& operator+=(const Polynomial& other);
  Polynomial& operator-=(const Polynomial& other);

  bool operator==(const Polynomial& other);
  bool operator!=(const Polynomial& other);

  Term LT() const;
  double LC() const;
  Monomial LM() const;

  bool IsZero() const;

  void Print(const std::vector<std::string>& names) const;

 private:
  size_t num_variables_;
  std::map<Monomial, double, PolynomialOrdering> terms_;

  void CheckCompatibility(const Polynomial& other) const;
  void CleanUp();
};

}  // namespace groebner::polynomial
