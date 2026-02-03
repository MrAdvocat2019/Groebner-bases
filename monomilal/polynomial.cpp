#include "polynomial.h"
#include <iostream>
#include <cmath>

namespace groebner {

Polynomial::Polynomial(Monomial m, OrderVariant strategy, double coeff): terms_(std::move(strategy)), num_variables_(m.GetNumVariables()){
    AddTerm(m, 1);
}

Polynomial::Polynomial(size_t num_variables, OrderVariant strategy): terms_(std::move(strategy)), num_variables_(num_variables){}

void Polynomial::AddTerm(Monomial monomial, double coefficient){
    terms_[std::move(monomial)] += coefficient;
}

Polynomial Polynomial::operator+(const Polynomial& other){
    CheckCompatibility(other);
    Polynomial res(*this);
    for (const auto& [monomial, coeff] : other.terms_){
        res.terms_[monomial] += coeff;
    }
    res.CleanUp();
    return res;
}

Polynomial Polynomial::operator-(const Polynomial& other){
    CheckCompatibility(other);
    Polynomial res(*this);
    for (const auto& [monomial, coeff] : other.terms_){
        res.terms_[monomial] -= coeff;
    }
    res.CleanUp();
    return res;
}

Polynomial Polynomial::operator*(const Polynomial& other){
    CheckCompatibility(other);
    Polynomial res(num_variables_, terms_.key_comp().GetStrategy());
    for (auto& [monomial1, coeff1] : terms_){
        for (auto& [monomial2, coeff2] : other.terms_){
            res.AddTerm(monomial1 * monomial2, coeff1 * coeff2);
        }
    }
    res.CleanUp();
    return res;
}

Polynomial& Polynomial::operator+=(const Polynomial& other){
    CheckCompatibility(other);
    for (const auto& [monomial, coeff] : other.terms_){
        terms_[monomial] += coeff;
    }
    CleanUp();
    return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& other){
    CheckCompatibility(other);
    for (const auto& [monomial, coeff] : other.terms_){
        terms_[monomial] -= coeff;
    }
    return *this;
}

std::pair<double, Monomial> Polynomial::LT() const {
    for (auto it = terms_.rbegin(); it != terms_.rend(); ++it) {
        if (std::abs(it->second) > 1e-9) {
            return {it->second, it->first};
        }
    }
    return {0, Monomial(num_variables_)};
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
    return Monomial(num_variables_);
}

void Polynomial::Print(const std::vector<std::string>& names) const {
    if(terms_.empty()){
        std::cout << "0" << std::endl;
        return;
    }
    bool first = true;
    for (auto it = terms_.rbegin(); it != terms_.rend(); ++it){
        const auto& [monomial, coeff] = *it;
        if(coeff != 0){
            if(!first) std::cout << " + ";
            std::cout << coeff;
            monomial.Print(names);
            first = false;
        }
    }
    std::cout << std::endl;
}

void Polynomial::CheckCompatibility(const Polynomial& other) const{
    assert(num_variables_ == other.num_variables_ && "Num variables is incompeteble");
    assert(terms_.key_comp() == other.terms_.key_comp() && "Not equal monomial ordering");
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
