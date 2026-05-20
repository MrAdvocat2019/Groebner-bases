#include "term.h"

namespace groebner {

    Term::Term(const Monomial& m, double coeff)
        : m_(m)
        , coeff_(coeff)
    {
    }

    double Term::GetCoeff() const {
        return coeff_;
    }

    const Monomial& Term::GetMonomial() const {
        return m_;
    }

    bool Term::operator==(const Term& other) {
        return m_ == other.m_ && coeff_ == other.coeff_;
    }

    Term& Term::operator*=(const Term& other) {
        m_ *= other.m_;
        coeff_ *= other.coeff_;
        return *this;
    }

    Term operator*(Term left, const Term& right) {
        left *= right;
        return left;
    }

    std::optional<Term> Term::DivideBy(const Term& other) {
        auto result_monomial = m_.DivideBy(other.m_);
        if (!result_monomial.has_value()) {
            return std::nullopt;
        }
        return Term(result_monomial.value(), coeff_ / other.coeff_);
    }

    Term Term::Lcm(const Term& other) const {
        return Term(m_.Lcm(other.m_), coeff_ * other.coeff_);
    }

} // namespace groebner
