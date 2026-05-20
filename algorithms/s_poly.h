#pragma once
#include "../monomial/polynomial.h"
namespace groebner::algorithms {
    groebner::Polynomial SPolynomial(const groebner::Polynomial& p1,
                                     const groebner::Polynomial& p2);
} // namespace groebner::algorithms
