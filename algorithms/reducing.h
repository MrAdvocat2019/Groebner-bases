#pragma once
#include <cassert>
#include <cstddef>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../monomial/polybuilder.h"
#include "../monomial/polynomial.h"
#include "algorithms/buchberger.h"
#include "division.h"
#include "monomial/monomial.h"

namespace groebner::algorithms {
namespace detail {
template <typename F>
std::vector<Polynomial<F>> MinimizeGroebner(
    const std::vector<Polynomial<F>>& g) {
  std::unordered_set<size_t> bad_indices;
  for (size_t i = 0; i < g.size(); ++i) {
    for (size_t j = 0; j < g.size(); ++j) {
      if (i == j) {
        continue;
      }
      if (g[i].LM() == g[j].LM()) {
        bad_indices.insert(std::min(i, j));
      } else if (g[i].LM().IsDivisibleBy(g[j].LM())) {
        bad_indices.insert(i);
      }
    }
  }
  std::vector<Polynomial<F>> res;
  for (size_t i = 0; i < g.size(); ++i) {
    if (!bad_indices.contains(i)) {
      res.push_back(g[i]);
    }
  }
  return res;
}
template <typename F>
std::vector<Polynomial<F>> ReduceMinimalGroebner(std::vector<Polynomial<F>> g) {
  if (g.size() <= 1) {
    return g;
  }
  for (size_t i = 0; i < g.size(); ++i) {
    std::swap(g[i], g.back());
    Polynomial<F> current = std::move(g.back());
    g.pop_back();
    Polynomial<F> reduced = Reduce(current, g).remainder;
    g.push_back(std::move(reduced));
    std::swap(g[i], g.back());
  }
  return g;
}
template <typename F>
std::vector<Polynomial<F>> NormalizeGroebner(std::vector<Polynomial<F>> g) {
  for (auto& p : g) {
    PolyBuilder<F> builder(p.GetOrder());
    p = p * builder.of(Term<F>(Monomial(), F(1) / p.LC()));
  }
  return g;
}
}  // namespace detail
template <typename F>
std::vector<Polynomial<F>> ReducedGroebner(std::vector<Polynomial<F>> g) {
  g = Buchberger(std::move(g));
  g = detail::MinimizeGroebner(g);
  g = detail::ReduceMinimalGroebner(std::move(g));
  g = detail::NormalizeGroebner(std::move(g));
  return g;
}
}  // namespace groebner::algorithms