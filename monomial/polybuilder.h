#pragma once
#include <cstddef>

#include "orders.h"
#include "polynomial.h"

namespace groebner {

template <typename Field>
class PolyBuilder {
 public:
  PolyBuilder(Order order, size_t num_ring_variables)
      : order_(order), num_ring_variables_(num_ring_variables) {}

  Polynomial<Field> of(Term<Field>&& t) const {
    return Polynomial<Field>(std::move(t), num_ring_variables_, order_);
  }

 private:
  Order order_;
  size_t num_ring_variables_;
};

}  // namespace groebner
