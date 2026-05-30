#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "monomial.h"
#include "polynomial.h"
#include "term.h"

namespace groebner {

class PolyPrinter {
 public:
  explicit PolyPrinter(std::vector<std::string> names,
                       std::ostream& out = std::cout)
      : names_(std::move(names)), out_(out) {}

  PolyPrinter& operator<<(const Monomial& m) {
    Write(m);
    return *this;
  }
  template <typename F>
  PolyPrinter& operator<<(const Term<F>& t) {
    Write(t);
    return *this;
  }
  template <typename F>
  PolyPrinter& operator<<(const Polynomial<F>& p) {
    Write(p);
    return *this;
  }
  template <typename T>
  PolyPrinter& operator<<(const T& v) {
    out_ << v;
    return *this;
  }

 private:
  void Write(const Monomial& m) {
    for (size_t i = 0; i < m.Exponents().size(); ++i) {
      if (m.Exponents()[i] != 0) {
        out_ << names_[i] << "^" << m.Exponents()[i];
      }
    }
  }
  template <typename F>
  void Write(const Term<F>& t) {
    out_ << t.Coeff();
    Write(t.M());
  }
  template <typename F>
  void Write(const Polynomial<F>& p) {
    if (p.Terms().empty()) {
      out_ << "0";
      return;
    }
    bool first = true;
    for (auto it = p.Terms().rbegin(); it != p.Terms().rend(); ++it) {
      const auto& [monomial, coeff] = *it;
      if (!first) {
        out_ << " + ";
      }
      out_ << coeff;
      Write(monomial);
      first = false;
    }
  }

  std::vector<std::string> names_;
  std::ostream& out_;
};

}  // namespace groebner