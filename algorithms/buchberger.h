#pragma once
#include <cassert>
#include <cstddef>
#include <set>
#include <utility>
#include <vector>

#include "../monomial/polynomial.h"
#include "division.h"
#include "monomial/monomial.h"
#include "s_poly.h"

namespace groebner::algorithms {
namespace detail {
using Pair = std::pair<size_t, size_t>;

inline Pair MakePair(size_t a, size_t b) {
  if (a < b) {
    return Pair{a, b};
  }
  return Pair{b, a};
}
template <typename F>
bool HasCoprimeLeadingMonomials(const Polynomial<F>& p1,
                                const Polynomial<F>& p2) {
  Monomial lcm = p1.LM().Lcm(p2.LM());
  return lcm == p1.LM() * p2.LM();
}
template <typename F>
bool IsRedundantPair(size_t i, size_t j,
                     const std::vector<Polynomial<F>>& basis,
                     const std::set<Pair>& pairs) {
  Monomial lcm_ij = basis[i].LM().Lcm(basis[j].LM());
  for (size_t k = 0; k < basis.size(); ++k) {
    if (k == i || k == j) {
      continue;
    }
    if (pairs.contains(MakePair(i, k)) || pairs.contains(MakePair(j, k))) {
      continue;
    }
    if (lcm_ij.IsDivisibleBy(basis[k].LM())) {
      return true;
    }
  }
  return false;
}
}  // namespace detail
template <typename F>
std::vector<Polynomial<F>> Buchberger(std::vector<Polynomial<F>> g) {
  assert(!g.empty() && "Dividers must not be empty");

  std::set<detail::Pair> pairs;
  for (size_t i = 0; i < g.size(); ++i) {
    for (size_t j = i + 1; j < g.size(); ++j) {
      pairs.emplace(i, j);
    }
  }

  while (!pairs.empty()) {
    auto [i, j] = *pairs.begin();
    pairs.erase(pairs.begin());

    if (detail::HasCoprimeLeadingMonomials(g[i], g[j])) {
      continue;
    }
    if (detail::IsRedundantPair(i, j, g, pairs)) {
      continue;
    }
    Polynomial<F> s_poly = SPolynomial(g[i], g[j]);
    Polynomial<F> remainder = Reduce(s_poly, g).remainder;

    if (!remainder.IsZero()) {
      size_t t = g.size();
      g.push_back(std::move(remainder));
      for (size_t k = 0; k < t; ++k) {
        pairs.emplace(k, t);
      }
    }
  }
  return g;
}
}  // namespace groebner::algorithms