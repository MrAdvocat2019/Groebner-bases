#include "monomial.h"
namespace groebner {
namespace monomial {
struct Term {
  double coeff;
  Monomial m;

  Term(const Monomial& m, double coeff);

  bool operator==(const Term& other);

  Term operator*(const Term& other);
  std::pair<bool, Term> CheckAndDivide(const Term& other);

  void CheckCompatibillity(const Term& other);
};
Term LCM(const Term& t1, const Term& t2);
}  // namespace monomial
}  // namespace groebner
