#pragma once
#include <cassert>

#include "../monomial/polybuilder.h"
#include "../monomial/polynomial.h"

namespace groebner::algorithms {

template <typename F>
Polynomial<F> SPolynomial(const Polynomial<F>& p1, const Polynomial<F>& p2) {
  assert(p1.GetOrder() == p2.GetOrder());
  Order order = p1.GetOrder();

  Term<F> lt_1 = p1.LT();
  Term<F> lt_2 = p2.LT();
  Term<F> lcm = lt_1.Lcm(lt_2);

  auto m1_res = lcm.DivideBy(lt_1);
  auto m2_res = lcm.DivideBy(lt_2);
  assert(m1_res.has_value() && m2_res.has_value());

  Term<F> m1 = m1_res.value();
  Term<F> m2 = m2_res.value();

  PolyBuilder<F> polynom(order);

  return polynom.of(std::move(m1)) * p1 - polynom.of(std::move(m2)) * p2;
}

}  // namespace groebner::algorithms
