#pragma once
#include <vector>
#include <utility>
#include <iostream>

namespace groebner {

class Monomial{
public:
    Monomial() : degree_(0), num_variables_(0) {}
    
    explicit Monomial(std::vector<size_t> exponents)
        : exponents_(std::move(exponents)), degree_(0) {
        num_variables_ = exponents_.size();
        for (size_t exp : exponents_) {
            degree_ += exp;
        }
    }
    
    size_t GetDegree() const {
        return degree_;
    }
    
    std::vector<size_t> GetExponents() const {
        return exponents_;
    }
    
    size_t GetNumVariables() const {
        return num_variables_;
    }

    bool operator==(const Monomial& other) const {
        return exponents_ == other.exponents_;
    }

    bool operator<(const Monomial& other) const {
        return exponents_ < other.exponents_;
    }

    void Print(const std::vector<std::string> names) const {
        for(size_t i = 0; i < exponents_.size(); ++i){
            if (exponents_[i] != 0) {
                std::cout << names[i] << "^" << exponents_[i];
            }
        }
    }
    
private:
    size_t degree_;
    size_t num_variables_;
    std::vector<size_t> exponents_;
};

} // namespace groebner