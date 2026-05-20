#pragma once
#include <cstddef>
#include <optional>
#include <vector>
#include "polynomial.h"
#include "orders.h"

namespace groebner {
    class PolyBuilder{
    public:
    PolyBuilder(Order order,  size_t num_ring_variables): order_(order), num_ring_variables_(num_ring_variables) {}
    Polynomial of(Term&& t) const {
        return Polynomial(std::move(t), num_ring_variables_, order_);
    }
    private:
    Order order_;
    size_t num_ring_variables_;
    };
}