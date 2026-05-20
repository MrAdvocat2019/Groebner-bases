#include "s_poly.h"

#include <cassert>

#include "monomial/orders.h"
#include "monomial/polybuilder.h"
#include "monomial/polynomial.h"

namespace groebner::algorithms {
    using groebner::Order;
    using groebner::PolyBuilder;
    using groebner::Polynomial;
    using groebner::Term;
    Polynomial SPolynomial(const Polynomial& p1, const Polynomial& p2) {
        assert(p1.GetOrder() == p2.GetOrder());
        Order order = p1.GetOrder();
        size_t num_ring_variables = p1.NumRingVariables();
        Term lt_1 = p1.LT();
        Term lt_2 = p2.LT();
        Term lcm = lt_1.Lcm(lt_2);

        auto m1_res = lcm.DivideBy(lt_1);
        auto m2_res = lcm.DivideBy(lt_2);
        assert(m1_res.has_value() && m2_res.has_value());

        Term m1 = m1_res.value();
        Term m2 = m2_res.value();

        PolyBuilder polynom(order, num_ring_variables);

        Polynomial res = polynom.of(std::move(m1)) * p1 -
                         polynom.of(std::move(m2)) * p2;
        return res;
    }
} // namespace groebner::algorithms
