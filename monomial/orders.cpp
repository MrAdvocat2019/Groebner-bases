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

Order Order::GrvLex() {
  return Order(Kind::GrvLex, [](const Monomial& lhs, const Monomial& rhs) {
    if (lhs.Degree() != rhs.Degree()) {
      return lhs.Degree() < rhs.Degree();
    }
    const auto& le = lhs.Exponents();
    const auto& re = rhs.Exponents();
    size_t n = std::max(le.size(), re.size());
    for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
      size_t a = 0;
      if (static_cast<size_t>(i) < le.size()) {
        a = le[i];
      }
      size_t b = 0;
      if (static_cast<size_t>(i) < re.size()) {
        b = re[i];
      }
      if (a != b) {
        return a > b;
      }
    }
    return false;
  });
}

}  // namespace groebner
