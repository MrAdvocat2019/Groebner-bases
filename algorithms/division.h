#pragma once
#include <cassert>
#include <cstddef>
#include <optional>
#include <vector>

#include "../monomial/polybuilder.h"
#include "../monomial/polynomial.h"

namespace groebner::algorithms {
namespace detail {
template <typename F>
std::optional<size_t> FindDivider(const Term<F>& lt,
                                  const std::vector<Polynomial<F>>& dividers) {
  for (size_t i = 0; i < dividers.size(); ++i) {
    Term<F> LT_f_i = dividers[i].LT();
    auto monomial_res = lt.DivideBy(LT_f_i);
    if (monomial_res.has_value()) {
      return i;
    }
  }
  return std::nullopt;
}
template <typename F>
void ReduceStep(const Polynomial<F>& divider, const PolyBuilder<F>& builder,
                Polynomial<F>* p, Polynomial<F>* quotient) {
  assert(p != nullptr && quotient != nullptr);
  Term<F> monomial_res = p->LT().DivideBy(divider.LT()).value();
  Polynomial<F> polynomial_res = builder.of(monomial_res);
  *quotient += polynomial_res;
  *p -= polynomial_res * divider;
}
template <typename F>
void UpdateRemainder(const PolyBuilder<F>& builder, Polynomial<F>* p,
                     Polynomial<F>* remainder) {
  assert(p != nullptr && remainder != nullptr);
  Polynomial<F> lt_poly = builder.of(p->LT());
  *remainder += lt_poly;
  *p -= lt_poly;
}

}  // namespace detail

template <typename F>
struct ReduceResult {
  std::vector<Polynomial<F>> quotients;
  Polynomial<F> remainder;
};

template <typename F>
ReduceResult<F> Reduce(const Polynomial<F>& f,
                       const std::vector<Polynomial<F>>& dividers) {
  size_t s = dividers.size();
  assert(s != 0 && "Dividers must not be empty");

  Order order = f.GetOrder();

  Polynomial<F> p = f;

  std::vector<Polynomial<F>> a(s, Polynomial<F>(order));
  Polynomial<F> r(order);

  PolyBuilder<F> builder(order);

  while (!p.IsZero()) {
    std::optional<size_t> found = detail::FindDivider(p.LT(), dividers);
    if (found) {
      detail::ReduceStep(dividers[*found], builder, &p, &a[*found]);
    } else {
      detail::UpdateRemainder(builder, &p, &r);
    }
  }
  return {std::move(a), std::move(r)};
}

}  // namespace groebner::algorithms
