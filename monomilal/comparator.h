#pragma once

#include "orders.h"
#include <variant>
#include <utility>

namespace groebner {

using OrderVariant = std::variant<LexOrder, GrLexOrder>;

class PolynomialOrdering {
    public:
        PolynomialOrdering(OrderVariant strategy): strategy_(std::move(strategy)){}

        bool operator()(const Monomial& lhs, const Monomial& rhs) const {
            return std::visit([&](const auto strategy){ return strategy(lhs, rhs); }, strategy_);
        }

        bool operator==(const PolynomialOrdering& other) const {
            return strategy_ == other.strategy_;
        }

        const OrderVariant& GetStrategy() const {
            return strategy_;
        }

    private:
        OrderVariant strategy_;
};

} // namespace groebner
