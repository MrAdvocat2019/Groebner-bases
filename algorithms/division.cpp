#include "division.h"
namespace groebner::algorithms {
    using groebner::Order;
    using groebner::Polynomial;
    using groebner::Term;
    DivisionResult Division(const Polynomial& f,
                            const std::vector<Polynomial>& dividers) {
        Polynomial p = f;
        size_t s = dividers.size();
        assert(s != 0 && "Dividers must not be empty");

        size_t num_ring_variables = dividers[0].NumRingVariables();
        Order order = dividers[0].GetOrder();

        std::vector<Polynomial> a(s, Polynomial(num_ring_variables, order));
        Polynomial r(num_ring_variables, order);
        while (!p.IsZero()) {
            size_t i = 0;
            bool flag = false;
            while (i < s && !flag) {
                Term LT_p = p.LT();
                Term LT_f_i = dividers[i].LT();
                auto monomial_res = LT_p.DivideBy(LT_f_i);
                if (monomial_res.has_value()) {
                    a[i] += Polynomial(monomial_res.value(), num_ring_variables, order);
                    p -= Polynomial(monomial_res.value(), num_ring_variables, order) * dividers[i];
                    flag = true;
                } else {
                    ++i;
                }
            }
            if (!flag) {
                Term LT_p = p.LT();
                r += Polynomial(LT_p, num_ring_variables, order);
                p -= Polynomial(LT_p, num_ring_variables, order);
            }
        }
        return {a, r};
    }
} // namespace groebner::algorithms
