#pragma once
#include "orders.h"
#include "polynomial.h"

namespace groebner {

template <typename F>
class PolyBuilder {
 public:
  explicit PolyBuilder(Order order) : order_(std::move(order)) {}

  Polynomial<F> of(Term<F> t) const {
    return Polynomial<F>(std::move(t), order_);
  }

 private:
  Order order_;
};

}  // namespace groebner
