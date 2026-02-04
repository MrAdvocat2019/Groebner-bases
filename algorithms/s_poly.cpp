#include "s_poly.h"
#include <cassert>
#include "monomial/comparator.h"
#include "monomial/polynomial.h"

namespace groebner::algorithms {
    using monomial::Term;
    using polynomial::Polynomial;
    Polynomial SPolynomial(const Polynomial& p1, const Polynomial& p2){
        assert(p1.GetStrategy() == p2.GetStrategy());
        ordering::OrderVariant strategy = p1.GetStrategy();
        Term lt_1 = p1.LT();
        Term lt_2 = p2.LT();
        Term lcm = monomial::LCMTerm(lt_1, lt_2);

        auto m1_res = lcm.CheckAndDivide(lt_1);
        auto m2_res = lcm.CheckAndDivide(lt_2);
        assert(m1_res.has_value() && m2_res.has_value());

        Term m1 = m1_res.value();
        Term m2 = m2_res.value();

        Polynomial res = Polynomial(m1, strategy) * p1 - Polynomial(m2, strategy) * p2;
        return res;
    }
}  // namespace groebner::algorithms