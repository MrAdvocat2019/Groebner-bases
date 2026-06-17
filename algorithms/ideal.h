#pragma once
#include <algorithm>
#include <vector>

#include "../monomial/polynomial.h"
#include "division.h"
#include "reducing.h"

namespace groebner::algorithms {

template <typename F>
class Ideal {
 public:
  Ideal() = default;
  Ideal(const std::vector<Polynomial<F>>& generators)
      : groebner_(ReducedGroebner(generators)) {}

  bool IsIn(Polynomial<F> f) const {
    return Reduce(f, groebner_).remainder.IsZero();
  }

  bool operator==(const Ideal<F>& other) const {
    if (groebner_.size() != other.groebner_.size()) {
      return false;
    }
    if (groebner_.empty()) {
      return true;
    }
    std::vector<Polynomial<F>> lhs = groebner_;
    std::vector<Polynomial<F>> rhs = other.groebner_;
    Order order = lhs.front().GetOrder();
    auto by_lm = [&order](const Polynomial<F>& a, const Polynomial<F>& b) {
      return order(a.LM(), b.LM());
    };
    std::sort(lhs.begin(), lhs.end(), by_lm);
    std::sort(rhs.begin(), rhs.end(), by_lm);
    return lhs == rhs;
  }

  bool operator!=(const Ideal<F>& other) const { return !(*this == other); }

 private:
  std::vector<Polynomial<F>> groebner_;
};
}  // namespace groebner::algorithms