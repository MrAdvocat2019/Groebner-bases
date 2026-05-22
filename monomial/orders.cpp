#include "orders.h"

namespace groebner {

Order Order::Lex() {
  return Order(Kind::Lex, [](const Monomial& lhs, const Monomial& rhs) {
    return lhs.Exponents() < rhs.Exponents();
  });
}

Order Order::GrLex() {
  return Order(Kind::GrLex, [](const Monomial& lhs, const Monomial& rhs) {
    if (lhs.Degree() != rhs.Degree()) {
      return lhs.Degree() < rhs.Degree();
    }
    return lhs.Exponents() < rhs.Exponents();
  });
}

}  // namespace groebner
