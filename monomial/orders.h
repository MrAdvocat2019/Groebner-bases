#pragma once
#include <functional>
#include <utility>

#include "monomial.h"

namespace groebner {

class Order {
 public:
  enum class Kind : unsigned char { Lex, GrLex };

  bool operator()(const Monomial& lhs, const Monomial& rhs) const {
    return cmp_(lhs, rhs);
  }

  bool operator==(const Order& other) const { return kind_ == other.kind_; }

  Kind GetKind() const { return kind_; }

  static Order Lex();
  static Order GrLex();

 private:
  Order(Kind kind, std::function<bool(const Monomial&, const Monomial&)> cmp)
      : kind_(kind), cmp_(std::move(cmp)) {}

  Kind kind_;
  std::function<bool(const Monomial&, const Monomial&)> cmp_;
};

}  // namespace groebner
