#pragma once
#include <cassert>
#include <vector>

#include "../monomial/monomial.h"
#include "../monomial/polynomial.h"
namespace groebner::algorithms {
using monomial::Monomial;
using polynomial::Polynomial;
struct DivisionResult {
  std::vector<Polynomial> quotients;
  Polynomial remainder;
};
DivisionResult Division(const Polynomial& f,
                        const std::vector<Polynomial>& dividers);
}  // namespace groebner::algorithms