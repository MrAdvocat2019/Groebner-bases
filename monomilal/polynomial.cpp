#include "polynomial.h"
#include <iostream>
#include <stdexcept>

namespace groebner {

Polynomial::Polynomial(size_t num_variables, OrderVariant strategy): terms_(std::move(strategy)), num_variables_(num_variables){}

void Polynomial::AddTerm(Monomial monomial, double coefficient){
    terms_[std::move(monomial)] += coefficient;
}

Polynomial Polynomial::operator+(const Polynomial& other){
    Polynomial res(*this);
    for (const auto& [monomial, coeff] : other.terms_){
        res.terms_[monomial] += coeff;
    }
    return res;
}

Polynomial Polynomial::operator-(const Polynomial& other){
    Polynomial res(*this);
    for (const auto& [monomial, coeff] : other.terms_){
        res.terms_[monomial] -= coeff;
    }
    return res;
}

// LT - Leading Term: возвращает пару (коэффициент, моном) старшего члена
std::pair<double, Monomial> Polynomial::LT() const {
    if (terms_.empty()) {
        throw std::runtime_error("Polynomial is empty, no leading term");
    }
    auto it = terms_.rbegin(); // Последний элемент (старший по порядку)
    return {it->second, it->first};
}

// LC - Leading Coefficient: возвращает коэффициент старшего члена
double Polynomial::LC() const {
    if (terms_.empty()) {
        throw std::runtime_error("Polynomial is empty, no leading coefficient");
    }
    return terms_.rbegin()->second;
}

// LM - Leading Monomial: возвращает моном старшего члена
Monomial Polynomial::LM() const {
    if (terms_.empty()) {
        throw std::runtime_error("Polynomial is empty, no leading monomial");
    }
    return terms_.rbegin()->first;
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

} // namespace groebner
