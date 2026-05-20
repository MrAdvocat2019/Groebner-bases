#pragma once
#include <map>

#include "orders.h"
#include "term.h"

namespace groebner {

    class Polynomial {
    public:
        Polynomial(Term t, size_t num_ring_variables, Order order);
        Polynomial(size_t num_ring_variables, Order order);

        size_t NumRingVariables() const;
        Order GetOrder() const;

        void AddTerm(const Term& term);

        Polynomial& operator+=(const Polynomial& other);
        Polynomial& operator-=(const Polynomial& other);
        Polynomial& operator*=(const Polynomial& other);

        bool operator==(const Polynomial& other);
        bool operator!=(const Polynomial& other);

        Term LT() const;
        double LC() const;
        Monomial LM() const;

        bool IsZero() const;

        void Print(const std::vector<std::string>& names) const;

        friend Polynomial operator+(Polynomial left, const Polynomial& right);
        friend Polynomial operator-(Polynomial left, const Polynomial& right);
        friend Polynomial operator*(const Polynomial& left, const Polynomial& right);

    private:
        void CheckCompatibility(const Polynomial& other) const;
        void CleanUp();

        size_t num_ring_variables_;
        std::map<Monomial, double, Order> terms_;
    };

} // namespace groebner
