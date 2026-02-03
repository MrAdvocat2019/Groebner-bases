#pragma once
#include "../monomial/monomial.h"
#include "../monomial/polynomial.h"
#include <cassert>
#include <vector>
namespace groebner{
namespace algorithms{
    using monomial::Monomial;
    using polynomial::Polynomial;
    struct DivisionResult{
        std::vector<Polynomial> quotients;
        Polynomial remainder;
    };
    DivisionResult Division(const Polynomial& f, const std::vector<Polynomial>& dividers);
} // namespace algorithms
} // namespace groebner