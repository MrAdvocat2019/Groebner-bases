#include <iostream>
#include <vector>

#include "algorithms/division.h"
#include "fields/rational.h"
#include "monomial/polynomial.h"

using namespace groebner;
using namespace groebner::algorithms;

using Poly = Polynomial<Rational>;
using Trm = Term<Rational>;

int main(int, char**) {
  std::cout << "=== Polynomial Ordering Demo ===\n\n";

  std::vector<std::string> var_names = {"x", "y"};

  std::cout << "Polynomial: x^5y^2 + x^6\n\n";

  std::cout << "--- LexOrder ---\n";
  Poly p_lex(2, Order::Lex());
  p_lex += Trm(Monomial::FromExponents({5, 2}), Rational(1));
  p_lex += Trm(Monomial::FromExponents({6, 0}), Rational(1));

  std::cout << "Polynomial: ";
  p_lex.Print(var_names);

  std::cout << "Leading Term (LT): coeff=" << p_lex.LT().GetCoeff()
            << ", degree=" << p_lex.LT().GetMonomial().Degree() << "\n";
  std::cout << "Leading Coefficient (LC): " << p_lex.LC() << "\n";
  std::cout << "Leading Monomial (LM) degree: " << p_lex.LM().Degree()
            << "\n\n";

  std::cout << "--- GrLexOrder ---\n";
  Poly p_grlex(2, Order::GrLex());
  p_grlex += Trm(Monomial::FromExponents({5, 2}), Rational(1));
  p_grlex += Trm(Monomial::FromExponents({6, 0}), Rational(1));

  std::cout << "Polynomial: ";
  p_grlex.Print(var_names);

  std::cout << "Leading Term (LT): coeff=" << p_grlex.LT().GetCoeff()
            << ", degree=" << p_grlex.LT().GetMonomial().Degree() << "\n";
  std::cout << "Leading Coefficient (LC): " << p_grlex.LC() << "\n";
  std::cout << "Leading Monomial (LM) degree: " << p_grlex.LM().Degree()
            << "\n\n";

  std::cout << "=== Arithmetic Operations Demo ===\n\n";

  Poly p1(2, Order::Lex());
  p1 += Trm(Monomial::FromExponents({2, 1}), Rational(3));
  p1 += Trm(Monomial::FromExponents({1, 0}), Rational(2));

  Poly p2(2, Order::Lex());
  p2 += Trm(Monomial::FromExponents({2, 1}), Rational(1));
  p2 += Trm(Monomial::FromExponents({0, 1}), Rational(4));

  std::cout << "p1 = ";
  p1.Print(var_names);
  std::cout << "p2 = ";
  p2.Print(var_names);
  std::cout << "\n";

  std::cout << "p1 + p2 = ";
  (p1 + p2).Print(var_names);

  std::cout << "p1 - p2 = ";
  (p1 - p2).Print(var_names);

  std::cout << "p1 * p2 = ";
  (p1 * p2).Print(var_names);

  Poly p3 = p1;
  p3 += p2;
  std::cout << "p3 = p1; p3 += p2; p3 = ";
  p3.Print(var_names);

  Poly p4 = p1;
  p4 -= p2;
  std::cout << "p4 = p1; p4 -= p2; p4 = ";
  p4.Print(var_names);

  std::cout << "\n=== Division Demo ===\n\n";

  Poly f(2, Order::Lex());
  f += Trm(Monomial::FromExponents({2, 1}), Rational(1));
  f += Trm(Monomial::FromExponents({1, 2}), Rational(1));
  f += Trm(Monomial::FromExponents({0, 2}), Rational(1));

  Poly f1(2, Order::Lex());
  f1 += Trm(Monomial::FromExponents({1, 1}), Rational(1));
  f1 += Trm(Monomial::FromExponents({0, 0}), Rational(-1));

  Poly f2(2, Order::Lex());
  f2 += Trm(Monomial::FromExponents({0, 2}), Rational(1));
  f2 += Trm(Monomial::FromExponents({0, 0}), Rational(-1));

  std::cout << "f = x^2y + xy^2 + y^2\n";
  std::cout << "Actual: ";
  f.Print(var_names);
  std::cout << "f1 = xy - 1\n";
  std::cout << "Actual: ";
  f1.Print(var_names);
  std::cout << "f2 = y^2 - 1\n";
  std::cout << "Actual: ";
  f2.Print(var_names);
  std::cout << "\n";

  auto result = Division(f, {f1, f2});

  std::cout << "Division f by {f1, f2}:\n";
  std::cout << "Expected: f = (x + y)(f1) + (1)(f2) + (x + y + 1)\n";
  for (size_t i = 0; i < result.quotients.size(); ++i) {
    std::cout << "Quotient " << i + 1 << " = ";
    result.quotients[i].Print(var_names);
  }
  std::cout << "Remainder = ";
  result.remainder.Print(var_names);

  return 0;
}
