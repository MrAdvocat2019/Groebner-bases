#include "term.h"
#include <cassert>
#include <iostream>

namespace groebner{
namespace monomial{

Term::Term(const Monomial& m, double coeff) : m(m), coeff(coeff) {}

bool Term::operator==(const Term& other) {
    CheckCompatibillity(other);
    return m == other.m && coeff == other.coeff;
}

Term Term::operator*(const Term& other) {
    CheckCompatibillity(other);
    return Term(m * other.m, coeff * other.coeff);
}

std::pair<bool, Term> Term::CheckAndDivide(const Term& other) {
    CheckCompatibillity(other);
    auto [divisible, result_monomial] = m.CheckAndDivide(other.m);
    if (!divisible) {
        return {false, Term(Monomial(), 0)};
    }
    return {true, Term(result_monomial, coeff / other.coeff)};
}

void Term::CheckCompatibillity(const Term& other) {
    assert(m.GetNumVariables() == other.m.GetNumVariables() && "Terms must have equal number of variables");
}

} // namespace monomial
} // namespace groebner
