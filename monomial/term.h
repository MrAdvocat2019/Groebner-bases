#pragma once

#include <optional>

#include "monomial.h"
namespace groebner {
    class Term {
    public:
        Term(const Monomial& m, double coeff);

        double GetCoeff() const;
        const Monomial& GetMonomial() const;

        bool operator==(const Term& other);

        Term& operator*=(const Term& other);
        std::optional<Term> DivideBy(const Term& other);
        Term Lcm(const Term& other) const;

        friend Term operator*(Term left, const Term& right);

    private:
        double coeff_;
        Monomial m_;
    };
} // namespace groebner
