#include <optional>

#include "monomial.h"
namespace groebner::monomial {
struct Term {
  double coeff;
  Monomial m;

  Term(const Monomial& m, double coeff);

  bool operator==(const Term& other);

  Term operator*(const Term& other);
  std::optional<Term> CheckAndDivide(const Term& other);

  void CheckCompatibillity(const Term& other);
};
Term LCMTerm(const Term& t1, const Term& t2);
}  // namespace groebner::monomial
