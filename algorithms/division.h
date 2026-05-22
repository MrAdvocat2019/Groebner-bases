#pragma once
#include <cassert>
#include <vector>

#include "../monomial/polynomial.h"

namespace groebner::algorithms {

template <typename Field>
struct DivisionResult {
  std::vector<Polynomial<Field>> quotients;
  Polynomial<Field> remainder;
};

template <typename Field>
DivisionResult<Field> Division(const Polynomial<Field>& f,
                               const std::vector<Polynomial<Field>>& dividers) {
  size_t s = dividers.size();
  assert(s != 0 && "Dividers must not be empty");

  size_t num_ring_variables = dividers[0].NumRingVariables();
  Order order = dividers[0].GetOrder();

  Polynomial<Field> p = f;
  std::vector<Polynomial<Field>> a(
      s, Polynomial<Field>(num_ring_variables, order));
  Polynomial<Field> r(num_ring_variables, order);

  while (!p.IsZero()) {
    size_t i = 0;
    bool flag = false;
    while (i < s && !flag) {
      Term<Field> LT_p = p.LT();
      Term<Field> LT_f_i = dividers[i].LT();
      auto monomial_res = LT_p.DivideBy(LT_f_i);
      if (monomial_res.has_value()) {
        a[i] +=
            Polynomial<Field>(monomial_res.value(), num_ring_variables, order);
        p -=
            Polynomial<Field>(monomial_res.value(), num_ring_variables, order) *
            dividers[i];
        flag = true;
      } else {
        ++i;
      }
    }
    if (!flag) {
      Term<Field> LT_p = p.LT();
      r += Polynomial<Field>(LT_p, num_ring_variables, order);
      p -= Polynomial<Field>(LT_p, num_ring_variables, order);
    }
  }
  return {std::move(a), std::move(r)};
}

}  // namespace groebner::algorithms
