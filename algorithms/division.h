#pragma once
#include <cassert>
#include <vector>

#include "../monomial/monomial.h"
#include "../monomial/polynomial.h"
namespace groebner::algorithms {
    struct DivisionResult {
        std::vector<groebner::Polynomial> quotients;
        groebner::Polynomial remainder;
    };
    DivisionResult Division(const groebner::Polynomial& f,
                            const std::vector<groebner::Polynomial>& dividers);
} // namespace groebner::algorithms
