#pragma once
#include "comparator.h"
#include "term.h"
#include <map>

namespace groebner {
namespace polynomial {

using monomial::Monomial;
using monomial::Term;
using ordering::OrderVariant;
using ordering::PolynomialOrdering;
using ordering::LexOrder;

class Polynomial{
    public:
        Polynomial(Term t, OrderVariant strategy);
        Polynomial(size_t num_variables, OrderVariant strategy = LexOrder{});

        size_t GetNumVariables() const;
        OrderVariant GetStrategy() const;

        void AddTerm(Term term);

        Polynomial operator+(const Polynomial& other);
        Polynomial operator-(const Polynomial& other);
        Polynomial operator*(const Polynomial& other);

        Polynomial& operator+=(const Polynomial& other);
        Polynomial& operator-=(const Polynomial& other);

        bool operator==(const Polynomial& other);
        bool operator!=(const Polynomial& other);

        Term LT() const;
        double LC() const;
        Monomial LM() const;

        bool IsZero() const;

        void Print(const std::vector<std::string>& names) const;
    private:
        size_t num_variables_;
        std::map<Monomial, double, PolynomialOrdering> terms_;

        void CheckCompatibility(const Polynomial& other) const;
        void CleanUp();
};

} // namespace polynomial
} // namespace groebner