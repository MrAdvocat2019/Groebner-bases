#include "polynomial.h"

#include <cassert>
#include <cmath>
#include <iostream>

namespace groebner {

    Polynomial::Polynomial(Term t, size_t num_ring_variables, Order order)
        : num_ring_variables_(num_ring_variables)
        , terms_(std::move(order)) {
        AddTerm(t);
    }

    Polynomial::Polynomial(size_t num_ring_variables, Order order)
        : num_ring_variables_(num_ring_variables)
        , terms_(std::move(order)) {}

    size_t Polynomial::NumRingVariables() const {
        return num_ring_variables_;
    }

    Order Polynomial::GetOrder() const {
        return terms_.key_comp();
    }

    void Polynomial::AddTerm(const Term& term) {
        terms_[term.GetMonomial()] += term.GetCoeff();
    }

    Polynomial& Polynomial::operator+=(const Polynomial& other) {
        CheckCompatibility(other);
        for (const auto& [monomial, coeff] : other.terms_) {
            terms_[monomial] += coeff;
        }
        CleanUp();
        return *this;
    }

    Polynomial& Polynomial::operator-=(const Polynomial& other) {
        CheckCompatibility(other);
        for (const auto& [monomial, coeff] : other.terms_) {
            terms_[monomial] -= coeff;
        }
        CleanUp();
        return *this;
    }

    Polynomial& Polynomial::operator*=(const Polynomial& other) {
        CheckCompatibility(other);
        Polynomial res(num_ring_variables_, terms_.key_comp());
        for (auto& [monomial1, coeff1] : terms_) {
            for (auto& [monomial2, coeff2] : other.terms_) {
                res.AddTerm(Term(monomial1 * monomial2, coeff1 * coeff2));
            }
        }
        res.CleanUp();
        *this = std::move(res);
        return *this;
    }

    Polynomial operator+(Polynomial left, const Polynomial& right) {
        left += right;
        return left;
    }

    Polynomial operator-(Polynomial left, const Polynomial& right) {
        left -= right;
        return left;
    }

    Polynomial operator*(const Polynomial& left, const Polynomial& right) {
        Polynomial result = left;
        result *= right;
        return result;
    }

    Term Polynomial::LT() const {
        for (auto it = terms_.rbegin(); it != terms_.rend(); ++it) {
            if (std::abs(it->second) > 1e-9) {
                return Term(it->first, it->second);
            }
        }
        return Term(Monomial(), 0);
    }

    double Polynomial::LC() const {
        for (auto it = terms_.rbegin(); it != terms_.rend(); ++it) {
            if (std::abs(it->second) > 1e-9) {
                return it->second;
            }
        }
        return 0;
    }

    Monomial Polynomial::LM() const {
        for (auto it = terms_.rbegin(); it != terms_.rend(); ++it) {
            if (std::abs(it->second) > 1e-9) {
                return it->first;
            }
        }
        return Monomial();
    }

    bool Polynomial::IsZero() const {
        if (terms_.empty()) {
            return true;
        }
        for (const auto& [m, c] : terms_) {
            if (std::abs(c) > 1e-9) {
                return false;
            }
        }
        return true;
    }

    void Polynomial::Print(const std::vector<std::string>& names) const {
        if (terms_.empty()) {
            std::cout << "0" << std::endl;
            return;
        }
        bool first = true;
        for (auto it = terms_.rbegin(); it != terms_.rend(); ++it) {
            const auto& [monomial, coeff] = *it;
            if (coeff != 0) {
                if (!first) {
                    std::cout << " + ";
                }
                std::cout << coeff;
                monomial.Print(names);
                first = false;
            }
        }
        std::cout << std::endl;
    }

    void Polynomial::CheckCompatibility(const Polynomial& other) const {
        assert(terms_.key_comp() == other.terms_.key_comp() &&
               "Not equal monomial ordering");
    }

    void Polynomial::CleanUp() {
        for (auto it = terms_.begin(); it != terms_.end();) {
            if (std::abs(it->second) < 1e-9) {
                it = terms_.erase(it);
            } else {
                ++it;
            }
        }
    }

} // namespace groebner
