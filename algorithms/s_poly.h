#pragma once
#include <cassert>

#include "../monomial/polybuilder.h"
#include "../monomial/polynomial.h"

namespace groebner::algorithms {

template <typename Field>
Polynomial<Field> SPolynomial(const Polynomial<Field>& p1,
                              const Polynomial<Field>& p2) {
  assert(p1.GetOrder() == p2.GetOrder());
  Order order = p1.GetOrder();
  size_t num_ring_variables = p1.NumRingVariables();

  Term<Field> lt_1 = p1.LT();
  Term<Field> lt_2 = p2.LT();
  Term<Field> lcm = lt_1.Lcm(lt_2);

  auto m1_res = lcm.DivideBy(lt_1);
  auto m2_res = lcm.DivideBy(lt_2);
  assert(m1_res.has_value() && m2_res.has_value());

  Term<Field> m1 = m1_res.value();
  Term<Field> m2 = m2_res.value();

  PolyBuilder<Field> polynom(order, num_ring_variables);

  return polynom.of(std::move(m1)) * p1 - polynom.of(std::move(m2)) * p2;
}

}  // namespace groebner::algorithms
