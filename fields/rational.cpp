#include "rational.h"
#include <cassert>
#include <cstdint>
#include <numeric>

namespace groebner {
    Rational::Rational(int64_t nominator, int64_t denominator)
        : numerator(nominator)
        , denominator(denominator)
    {
        Normalize();
    }
    Rational::Rational(int64_t num)
        : numerator(num)
        , denominator(1)
    {
    }

    int64_t Rational::Numerator() const {
        return numerator;
    }

    int64_t Rational::Denominator() const {
        return denominator;
    }

    Rational Rational::operator-() const {
        return Rational(-numerator, denominator);
    }

    Rational& Rational::operator+=(const Rational& other) {
        int64_t new_denominator = std::lcm(denominator, other.denominator);
        int64_t new_numerator = numerator * (new_denominator / denominator) + other.numerator * (new_denominator / other.denominator);
        numerator = new_numerator;
        denominator = new_denominator;
        Normalize();
        return *this;
    }

    Rational& Rational::operator-=(const Rational& other) {
        int64_t new_denominator = std::lcm(denominator, other.denominator);
        int64_t new_numerator = numerator * (new_denominator / denominator) - other.numerator * (new_denominator / other.denominator);
        numerator = new_numerator;
        denominator = new_denominator;
        Normalize();
        return *this;
    }

    Rational& Rational::operator*=(const Rational& other) {
        int64_t new_denominator = denominator * other.denominator;
        int64_t new_numerator = numerator * other.numerator;
        numerator = new_numerator;
        denominator = new_denominator;
        Normalize();
        return *this;
    }

    Rational& Rational::operator/=(const Rational& other) {
        int64_t new_denominator = denominator * other.numerator;
        int64_t new_numerator = numerator * other.denominator;
        numerator = new_numerator;
        denominator = new_denominator;
        Normalize();
        return *this;
    }
    

} // namespace groebner
