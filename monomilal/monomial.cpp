#include "monomial.h"
#include <cassert>

namespace groebner {

Monomial::Monomial() : degree_(0), num_variables_(0) {}

Monomial::Monomial(size_t num_variables) : degree_(0), num_variables_(num_variables) {}

Monomial::Monomial(std::vector<size_t> exponents)
    : exponents_(std::move(exponents)), degree_(0) {
    num_variables_ = exponents_.size();
    for (size_t exp : exponents_) {
        degree_ += exp;
    }
}

size_t Monomial::GetDegree() const {
    return degree_;
}

std::vector<size_t> Monomial::GetExponents() const {
    return exponents_;
}

size_t Monomial::GetNumVariables() const {
    return num_variables_;
}

bool Monomial::operator==(const Monomial& other) const {
    return exponents_ == other.exponents_;
}

bool Monomial::operator<(const Monomial& other) const {
    return exponents_ < other.exponents_;
}

Monomial Monomial::operator*(const Monomial& other) const {
    assert(num_variables_ == other.num_variables_ && "Incorrect monomial multiplying, monomials should have equal num_variables");
    Monomial res(*this);
    res.degree_ += other.degree_;

    for (size_t i = 0; i < num_variables_; ++i){
        res.exponents_[i] += other.exponents_[i];
    }
    return res;
}

std::pair<bool, Monomial> Monomial::CheckAndDivide(const Monomial& other) const{
    assert(num_variables_ == other.num_variables_ && "Incorrect monomial dividing, monomials should have equal num_variables");
    Monomial res(*this);
    res.degree_ -= other.degree_;
    for(size_t i = 0; i < num_variables_; ++i){
        if(res.exponents_[i] < other.exponents_[i]){
            return {false, Monomial()};
        }
        res.exponents_[i] -= other.exponents_[i];
    }
    return {true, res};
}

void Monomial::Print(const std::vector<std::string> names) const {
    if(degree_ == 0){
        std::cout << 1 << " ";
    }
    else{
        for(size_t i = 0; i < exponents_.size(); ++i){
            if (exponents_[i] != 0) {
                std::cout << names[i] << "^" << exponents_[i];
            }
        }
    }
    
}

} // namespace groebner
