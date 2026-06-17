#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "algorithms/buchberger.h"
#include "algorithms/division.h"
#include "algorithms/ideal.h"
#include "algorithms/reducing.h"
#include "algorithms/s_poly.h"
#include "fields/rational.h"
#include "fields/zp.h"
#include "monomial/monomial.h"
#include "monomial/orders.h"
#include "monomial/polynomial.h"
#include "monomial/term.h"
#include "tests/cyclic.h"
#include "tests/timer.h"

using groebner::Monomial;
using groebner::Order;
using groebner::Polynomial;
using groebner::Rational;
using groebner::Term;
using groebner::algorithms::Buchberger;
using groebner::algorithms::Ideal;
using groebner::algorithms::Reduce;
using groebner::algorithms::ReducedGroebner;
using groebner::algorithms::SPolynomial;

using Poly = Polynomial<Rational>;
using Trm = Term<Rational>;
using Coeff = std::pair<std::vector<size_t>, int>;

namespace {

Monomial Mono(std::vector<size_t> exponents) {
  return Monomial::FromExponents(std::move(exponents));
}

Poly MakePoly(Order order, std::initializer_list<Coeff> terms) {
  Poly p(order);
  for (const auto& [exponents, coeff] : terms) {
    p += Trm(Monomial::FromExponents(exponents), Rational(coeff));
  }
  return p;
}

int g_checks = 0;
int g_failures = 0;

void Check(bool condition, const std::string& name) {
  ++g_checks;
  if (condition) {
    std::cout << "[ PASS ] " << name << "\n";
  } else {
    ++g_failures;
    std::cout << "[ FAIL ] " << name << "\n";
  }
}

void TestRational() {
  Check(Rational(2, 4) == Rational(1, 2), "2/4 normalizes to 1/2");
  Check(Rational(1, -2) == Rational(-1, 2), "sign moves to numerator");
  Check(Rational(1, -2).Denominator() == 2, "denominator stays positive");
  Check(Rational(0, 5) == Rational(0), "0/5 normalizes to 0");
  Check(Rational(1, 2) + Rational(1, 3) == Rational(5, 6), "1/2 + 1/3 = 5/6");
  Check(Rational(1, 6) - Rational(1, 2) == Rational(-1, 3), "1/6 - 1/2 = -1/3");
  Check(Rational(2, 3) * Rational(3, 4) == Rational(1, 2), "2/3 * 3/4 = 1/2");
  Check(Rational(1, 2) / Rational(3, 4) == Rational(2, 3), "1/2 / 3/4 = 2/3");
  Check(-Rational(1, 2) == Rational(-1, 2), "unary minus");
  Check(Rational(1, 3) < Rational(1, 2), "1/3 < 1/2");
}

void TestMonomial() {
  Check(Mono({2, 3, 0}).Degree() == 5, "degree is sum of exponents");
  Check(Mono({2, 3, 0}).Size() == 2, "trailing zeros are dropped");
  Check(Mono({2, 0}) == Mono({2}), "x^2 canonical form");
  Check(Mono({0, 0, 0}).Size() == 0, "constant monomial is empty");

  Check(Mono({2, 1}) * Mono({1, 3}) == Mono({3, 4}), "x^2y * xy^3 = x^3y^4");
  Check(Mono({1}) * Mono({0, 1}) == Mono({1, 1}), "x * y = xy (mixed sizes)");

  Check(Mono({3, 4}).IsDivisibleBy(Mono({2, 1})), "x^3y^4 divisible by x^2y");
  Check(!Mono({2, 1}).IsDivisibleBy(Mono({3})), "x^2y not divisible by x^3");
  Check(Mono({3, 4}) / Mono({2, 1}) == Mono({1, 3}), "x^3y^4 / x^2y = xy^3");

  Check(Mono({2, 1}).Lcm(Mono({1, 3})) == Mono({2, 3}),
        "lcm(x^2y, xy^3)=x^2y^3");
  Check(Mono({2, 1}).Lcm(Mono({})) == Mono({2, 1}), "lcm with constant");
}

void TestOrder() {
  Monomial x2 = Mono({2});
  Monomial y3 = Mono({0, 3});

  Order lex = Order::Lex();
  Check(lex(y3, x2) && !lex(x2, y3), "Lex: x^2 > y^3");

  Order grlex = Order::GrLex();
  Check(grlex(x2, y3) && !grlex(y3, x2), "GrLex: x^2 < y^3");

  Check(Order::Lex().GetKind() == Order::Kind::Lex, "GetKind reports Lex");
  Check(Order::Lex() == Order::Lex(), "orders of same kind are equal");
  Check(!(Order::Lex() == Order::GrLex()), "different kinds are unequal");
}

void TestTerm() {
  Trm a(Mono({2}), Rational(2));
  Trm b(Mono({1, 1}), Rational(3));
  Check((a * b) == Trm(Mono({3, 1}), Rational(6)), "2x^2 * 3xy = 6x^3y");

  Trm big(Mono({3, 1}), Rational(6));
  Trm small(Mono({1}), Rational(3));
  Check(big.IsDivisibleBy(small), "6x^3y divisible by 3x");
  Check((big / small) == Trm(Mono({2, 1}), Rational(2)), "6x^3y / 3x = 2x^2y");
  Check(a.Lcm(b) == Trm(Mono({2, 1}), Rational(1)), "term lcm is monic");
}

void TestPolynomial() {
  Order order = Order::Lex();

  Poly x_plus_y = MakePoly(order, {{{1, 0}, 1}, {{0, 1}, 1}});
  Poly x_minus_y = MakePoly(order, {{{1, 0}, 1}, {{0, 1}, -1}});

  Check(x_plus_y + x_minus_y == MakePoly(order, {{{1, 0}, 2}}),
        "(x+y)+(x-y) = 2x");

  Poly product = x_plus_y * x_minus_y;
  Check(product == MakePoly(order, {{{2, 0}, 1}, {{0, 2}, -1}}),
        "(x+y)(x-y) = x^2 - y^2");

  Check((x_plus_y - x_plus_y).IsZero(), "p - p is zero");
  Check(!x_plus_y.IsZero(), "x + y is not zero");

  Check(product.LM() == Mono({2}), "LM(x^2 - y^2) = x^2");
  Check(product.LC() == Rational(1), "LC(x^2 - y^2) = 1");
  Check(product.LT() == Trm(Mono({2}), Rational(1)), "LT(x^2 - y^2) = x^2");

  Poly acc(order);
  acc += Trm(Mono({1}), Rational(1));
  acc += Trm(Mono({1}), Rational(1));
  Check(acc == MakePoly(order, {{{1}, 2}}), "x + x = 2x");
  acc += Trm(Mono({1}), Rational(-2));
  Check(acc.IsZero(), "coefficients cancelling to 0");
}

void TestSPolynomial() {
  Order order = Order::Lex();
  Poly p1 = MakePoly(order, {{{2, 0}, 1}, {{0, 0}, 1}});
  Poly p2 = MakePoly(order, {{{1, 1}, 1}, {{0, 0}, 1}});

  Check(SPolynomial(p1, p2) == MakePoly(order, {{{1, 0}, -1}, {{0, 1}, 1}}),
        "S(x^2+1, xy+1) = -x + y");
}

void TestReduce() {
  Order order = Order::Lex();
  Poly f = MakePoly(order, {{{2, 1}, 1}, {{1, 2}, 1}, {{0, 2}, 1}});
  Poly f1 = MakePoly(order, {{{1, 1}, 1}, {{0, 0}, -1}});
  Poly f2 = MakePoly(order, {{{0, 2}, 1}, {{0, 0}, -1}});

  auto res = Reduce(f, {f1, f2});
  Check(
      res.remainder == MakePoly(order, {{{1, 0}, 1}, {{0, 1}, 1}, {{0, 0}, 1}}),
      "remainder of (x^2y+xy^2+y^2) by (xy-1, y^2-1) = x + y + 1");
  Check(res.quotients[0] == MakePoly(order, {{{1, 0}, 1}, {{0, 1}, 1}}),
        "quotient a1 = x + y");
  Check(res.quotients[1] == MakePoly(order, {{{0, 0}, 1}}), "a2 = 1");
  Check(res.quotients[0] * f1 + res.quotients[1] * f2 + res.remainder == f,
        "f = a1*f1 + a2*f2 + r");

  Check(Reduce(MakePoly(order, {{{2, 1}, 1}, {{1, 0}, -1}}), {f1})
            .remainder.IsZero(),
        "x*(xy-1) reduces to 0");
}

bool AllReduceToZero(const std::vector<Poly>& polys,
                     const std::vector<Poly>& basis) {
  for (const auto& p : polys) {
    if (!Reduce(p, basis).remainder.IsZero()) {
      return false;
    }
  }
  return true;
}

bool IsGroebnerBasis(const std::vector<Poly>& basis) {
  for (size_t i = 0; i < basis.size(); ++i) {
    for (size_t j = i + 1; j < basis.size(); ++j) {
      if (!Reduce(SPolynomial(basis[i], basis[j]), basis).remainder.IsZero()) {
        return false;
      }
    }
  }
  return true;
}

bool IsReducedBasis(const std::vector<Poly>& basis) {
  for (size_t i = 0; i < basis.size(); ++i) {
    if (basis[i].LC() != Rational(1)) {
      return false;
    }
    for (const auto& [monomial, coeff] : basis[i].Terms()) {
      for (size_t j = 0; j < basis.size(); ++j) {
        if (i != j && monomial.IsDivisibleBy(basis[j].LM())) {
          return false;
        }
      }
    }
  }
  return true;
}

void TestBuchberger() {
  Order order = Order::GrLex();
  std::vector<Poly> gens = {
      MakePoly(order, {{{3, 0}, 1}, {{1, 1}, -2}}),
      MakePoly(order, {{{2, 1}, 1}, {{0, 2}, -2}, {{1, 0}, 1}}),
  };

  std::vector<Poly> basis = Buchberger(gens);
  Check(IsGroebnerBasis(basis), "Buchberger output is a Groebner basis");
  Check(AllReduceToZero(gens, basis), "generators reduce to 0 mod basis");

  std::vector<Poly> reduced = ReducedGroebner(gens);
  Check(IsGroebnerBasis(reduced), "ReducedGroebner output is a Groebner basis");
  Check(AllReduceToZero(gens, reduced), "generators reduce to 0 mod reduced");
  Check(IsReducedBasis(reduced), "ReducedGroebner output is monic and reduced");
  Check(Ideal<Rational>(gens) == Ideal<Rational>(reduced),
        "reduced basis spans the original ideal");
}

void TestMembershipPrincipal() {
  Order order = Order::Lex();
  Ideal<Rational> ideal({MakePoly(order, {{{2}, 1}, {{0}, -1}})});

  Check(ideal.IsIn(MakePoly(order, {{{3}, 1}, {{1}, -1}})),
        "x^3 - x in (x^2 - 1)");
  Check(ideal.IsIn(MakePoly(order, {{{2}, 1}, {{0}, -1}})),
        "generator x^2 - 1 in (x^2 - 1)");
  Check(ideal.IsIn(Poly(order)), "0 in (x^2 - 1)");
  Check(!ideal.IsIn(MakePoly(order, {{{2}, 1}, {{0}, 1}})),
        "x^2 + 1 not in (x^2 - 1)");
}

void TestMembershipMultivariate() {
  Order order = Order::Lex();
  Ideal<Rational> ideal({
      MakePoly(order, {{{1, 0}, 1}, {{0, 1}, -1}}),
      MakePoly(order, {{{0, 2}, 1}, {{0, 1}, -1}}),
  });

  Check(ideal.IsIn(MakePoly(order, {{{2, 0}, 1}, {{0, 1}, -1}})),
        "x^2 - y in (x - y, y^2 - y)");
  Check(!ideal.IsIn(MakePoly(order, {{{1, 0}, 1}})),
        "x not in (x - y, y^2 - y)");
  Check(!ideal.IsIn(MakePoly(order, {{{0, 0}, 1}})),
        "1 not in (x - y, y^2 - y)");
}

void TestIdealEquality() {
  Order order = Order::Lex();

  Ideal<Rational> xy({
      MakePoly(order, {{{1, 0}, 1}}),
      MakePoly(order, {{{0, 1}, 1}}),
  });

  Ideal<Rational> sum_diff({
      MakePoly(order, {{{1, 0}, 1}, {{0, 1}, 1}}),
      MakePoly(order, {{{1, 0}, 1}, {{0, 1}, -1}}),
  });
  Check(sum_diff == xy, "(x + y, x - y) == (x, y)");

  Ideal<Rational> scaled_reordered({
      MakePoly(order, {{{0, 1}, 3}}),
      MakePoly(order, {{{1, 0}, 2}}),
  });
  Check(scaled_reordered == xy, "(3y, 2x) == (x, y)");

  Ideal<Rational> redundant({
      MakePoly(order, {{{1, 0}, 1}}),
      MakePoly(order, {{{0, 1}, 1}}),
      MakePoly(order, {{{1, 0}, 1}, {{0, 1}, 1}}),
  });
  Check(redundant == xy, "(x, y, x + y) == (x, y)");

  Check(xy == xy, "(x, y) == (x, y)");
}

void TestIdealInequality() {
  Order order = Order::Lex();

  Ideal<Rational> sq({MakePoly(order, {{{2}, 1}, {{0}, -1}})});
  Ideal<Rational> lin({MakePoly(order, {{{1}, 1}, {{0}, -1}})});
  Check(sq != lin, "(x^2 - 1) != (x - 1)");

  Ideal<Rational> x({MakePoly(order, {{{1, 0}, 1}})});
  Ideal<Rational> xy({
      MakePoly(order, {{{1, 0}, 1}}),
      MakePoly(order, {{{0, 1}, 1}}),
  });
  Check(x != xy, "(x) != (x, y)");
}

void BenchmarkCyclic() {
  using groebner::testing::MakeCyclic;
  using groebner::testing::ScopedTimer;
  using F = groebner::Zp<11>;

  std::cout << "\n=== Cyclic-n benchmark (Zp<11>, GrLex) ===\n";
  for (size_t n = 1; n <= 5; ++n) {
    std::vector<Polynomial<F>> gens = MakeCyclic<F>(n, Order::GrLex());
    std::vector<Polynomial<F>> basis;
    {
      ScopedTimer timer("  Cyclic-" + std::to_string(n));
      basis = Buchberger<F>(gens);
    }
    std::cout << "    -> basis size = " << basis.size() << "\n";
  }
}

}  // namespace

int main() {
  TestRational();
  TestMonomial();
  TestOrder();
  TestTerm();
  TestPolynomial();
  TestSPolynomial();
  TestReduce();
  TestBuchberger();
  TestMembershipPrincipal();
  TestMembershipMultivariate();
  TestIdealEquality();
  TestIdealInequality();

  std::cout << "\n"
            << (g_checks - g_failures) << "/" << g_checks << " checks passed\n";

  BenchmarkCyclic();

  return g_failures == 0 ? 0 : 1;
}
