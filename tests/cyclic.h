#pragma once
#include <cstddef>
#include <vector>

#include "monomial/monomial.h"
#include "monomial/orders.h"
#include "monomial/polynomial.h"
#include "monomial/term.h"

namespace groebner::testing {
namespace detail {
template <typename F>
Term<F> CyclicTerm(size_t start_index, size_t window_size,
                   size_t num_variables) {
  auto v = std::vector<size_t>(num_variables);
  for (size_t i = 0; i < window_size; ++i) {
    v[(start_index + i) % num_variables] = 1;
  }
  Monomial m = Monomial::FromExponents(std::move(v));
  return Term<F>(m, F(1));
}
}  // namespace detail

template <typename F>
std::vector<Polynomial<F>> MakeCyclic(size_t n, Order order = Order::Lex()) {
  std::vector<Polynomial<F>> res;
  for (size_t i = 1; i <= n - 1; ++i) {
    Polynomial<F> poly(order);
    for (size_t j = 0; j < n; ++j) {
      poly += detail::CyclicTerm<F>(j, i, n);
    }
    res.push_back(std::move(poly));
  }
  Polynomial<F> last(order);
  last += detail::CyclicTerm<F>(0, n, n);
  last += Term<F>(Monomial(), F(-1));
  res.push_back(std::move(last));
  return res;
}
}  // namespace groebner::testing