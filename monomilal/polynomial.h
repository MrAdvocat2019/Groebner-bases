#pragma once
#include "comparator.h"
#include <map>

namespace groebner {

class Polynomial{
    public:
        Polynomial(size_t num_variables, OrderVariant strategy = LexOrder{});
        void AddTerm(Monomial monomial, double coefficient);

        Polynomial operator+(const Polynomial& other);
        Polynomial operator-(const Polynomial& other);
        Polynomial operator*(const Polynomial& other);

        bool operator==(const Polynomial& other);
        bool operator!=(const Polynomial& other);

        std::pair<double, Monomial> LT() const;
        double LC() const;
        Monomial LM() const;

        void Print(const std::vector<std::string>& names) const;
    private:
        size_t num_variables_;
        std::map<Monomial, double, PolynomialOrdering> terms_;
};

} // namespace groebner