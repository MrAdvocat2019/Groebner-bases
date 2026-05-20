#pragma once
#include <cstdint>
#include <numeric>
#include <stdexcept>

namespace groebner {
    class Rational {
    public:
        Rational() = default;
        Rational(int64_t numerator, int64_t denominator);
        Rational(int64_t num);

        int64_t Numerator() const;
        int64_t Denominator() const;

        Rational operator-() const;

        Rational& operator+=(const Rational& other);
        Rational& operator-=(const Rational& other);
        Rational& operator*=(const Rational& other);
        Rational& operator/=(const Rational& other);

        bool operator==(const Rational& lhs);
        bool operator!=(const Rational& lhs);
        bool operator<(const Rational& lhs);

        friend Rational operator+(const Rational& lhs, const Rational& rhs);
        friend Rational operator-(const Rational& lhs, const Rational& rhs);
        friend Rational operator*(const Rational& lhs, const Rational& rhs);
        friend Rational operator/(const Rational& lhs, const Rational& rhs);

    private:
        void Normalize();
        bool IsZero();

        int64_t numerator = 0;
        int64_t denominator = 1;
    };
} // namespace groebner
