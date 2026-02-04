#include "term.h"

#include <cassert>

#include "monomial/monomial.h"

namespace groebner::monomial {

Term::Term(const Monomial& m, double coeff) : m(m), coeff(coeff) {}

bool Term::operator==(const Term& other) {
  CheckCompatibillity(other);
  return m == other.m && coeff == other.coeff;
}

Term Term::operator*(const Term& other) {
  CheckCompatibillity(other);
  return Term(m * other.m, coeff * other.coeff);
}

std::optional<Term> Term::CheckAndDivide(const Term& other) {
  CheckCompatibillity(other);
  auto result_monomial = m.CheckAndDivide(other.m);
  if (!result_monomial.has_value()) {
    return std::nullopt;
  }
  return Term(result_monomial.value(), coeff / other.coeff);
}

void Term::CheckCompatibillity(const Term& other) {
  assert(m.GetNumVariables() == other.m.GetNumVariables() &&
         "Terms must have equal number of variables");
}

Term LCMTerm(const Term& t1, const Term& t2) {
  return Term(LCMMonomial(t1.m, t2.m), t1.coeff * t2.coeff);
}
}  // namespace groebner::monomial
