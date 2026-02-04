#include "division.h"
namespace groebner {
namespace algorithms {
using monomial::Monomial;
using monomial::Term;
using ordering::OrderVariant;
using polynomial::Polynomial;
DivisionResult Division(const Polynomial& f,
                        const std::vector<Polynomial>& dividers) {
  Polynomial p = f;
  size_t s = dividers.size();
  assert(s != 0 && "Dividers must not be empty");

  size_t num_variables = dividers[0].GetNumVariables();
  OrderVariant strategy = dividers[0].GetStrategy();

  std::vector<Polynomial> a(s, Polynomial(num_variables, strategy));
  Polynomial r(num_variables, strategy);
  while (!p.IsZero()) {
    size_t i = 0;
    bool flag = false;
    while (i < s && !flag) {
      Term LT_p = p.LT();
      Term LT_f_i = dividers[i].LT();
      auto monomial_res = LT_p.CheckAndDivide(LT_f_i);
      if (monomial_res.first) {
        a[i] += Polynomial(monomial_res.second, strategy);
        p -= Polynomial(monomial_res.second, strategy) * dividers[i];
        flag = true;
      } else {
        ++i;
      }
    }
    if (!flag) {
      Term LT_p = p.LT();
      r += Polynomial(LT_p, strategy);
      p -= Polynomial(LT_p, strategy);
    }
  }
  return {a, r};
}
}  // namespace algorithms
}  // namespace groebner
