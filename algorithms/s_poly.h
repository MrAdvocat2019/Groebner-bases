#pragma once
#include "../monomial/polynomial.h"
namespace groebner::algorithms {
using polynomial::Polynomial;
Polynomial SPolynomial(const Polynomial& p1, const Polynomial& p2);
}  // namespace groebner::algorithms
