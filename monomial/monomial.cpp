#include "monomial.h"

#include <algorithm>
#include <iostream>

namespace groebner {

    Monomial::Monomial()
        : exponents_()
        , degree_(0)
        , size_(0)
    {
    }

    Monomial::Monomial(std::vector<size_t>&& clean_exponents)
        : exponents_(std::move(clean_exponents))
        ,
        degree_(ComputeDegree(exponents_))
        ,
        size_(exponents_.size())
    {
    }

    Monomial Monomial::FromExponents(std::vector<size_t>&& exponents) {
        RemoveTrailingZeroes(exponents);
        return Monomial(std::move(exponents));
    }

    Monomial Monomial::FromExponents(const std::vector<size_t>& exponents) {
        return FromExponents(std::vector<size_t>(exponents));
    }

    size_t Monomial::Degree() const {
        return degree_;
    }

    const std::vector<size_t>& Monomial::Exponents() const {
        return exponents_;
    }

    size_t Monomial::Size() const {
        return size_;
    }

    bool Monomial::operator==(const Monomial& other) const {
        return degree_ == other.degree_ && size_ == other.size_ && exponents_ == other.exponents_;
    }

    bool Monomial::operator!=(const Monomial& other) const {
        return degree_ != other.degree_ || size_ != other.size_ || exponents_ != other.exponents_;
    }

    Monomial& Monomial::operator*=(const Monomial& other) {
        if (exponents_.size() < other.exponents_.size()) {
            exponents_.resize(other.exponents_.size(), 0);
        }
        degree_ += other.degree_;
        for (size_t i = 0; i < other.exponents_.size(); ++i) {
            exponents_[i] += other.exponents_[i];
        }
        size_ = exponents_.size();
        return *this;
    }

    Monomial operator*(Monomial left, const Monomial& right) {
        left *= right;
        return left;
    }

    bool Monomial::IsDivisibleBy(const Monomial& other) const {
        if (other.size_ > size_) {
            return false;
        }
        for (size_t i = 0; i < other.exponents_.size(); ++i) {
            if (exponents_[i] < other.exponents_[i]) {
                return false;
            }
        }
        return true;
    }

    std::optional<Monomial> Monomial::DivideBy(const Monomial& other) const {
        if (!IsDivisibleBy(other)) {
            return std::nullopt;
        }
        std::vector<size_t> result(exponents_);
        for (size_t i = 0; i < other.exponents_.size(); ++i) {
            result[i] -= other.exponents_[i];
        }
        RemoveTrailingZeroes(result);
        return Monomial(std::move(result));
    }

    void Monomial::Print(const std::vector<std::string>& names) const {
        for (size_t i = 0; i < exponents_.size(); ++i) {
            if (exponents_[i] != 0) {
                std::cout << names[i] << "^" << exponents_[i];
            }
        }
    }

    Monomial Monomial::Lcm(const Monomial& other) const {
        size_t max_size = std::max(exponents_.size(), other.exponents_.size());
        std::vector<size_t> result_exponents(max_size);
        for (size_t i = 0; i < max_size; ++i) {
            size_t a = i < exponents_.size() ? exponents_[i] : 0;
            size_t b = i < other.exponents_.size() ? other.exponents_[i] : 0;
            result_exponents[i] = std::max(a, b);
        }
        RemoveTrailingZeroes(result_exponents);
        return Monomial(std::move(result_exponents));
    }

    void Monomial::RemoveTrailingZeroes(std::vector<size_t>& v) {
        while (!v.empty() && v.back() == 0) {
            v.pop_back();
        }
    }

    size_t Monomial::ComputeDegree(const std::vector<size_t>& v) {
        size_t sum = 0;
        for (size_t exp : v) {
            sum += exp;
        }
        return sum;
    }

} // namespace groebner
