#pragma once
#include <cstdint>
#include <iosfwd>
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

  bool operator==(const Rational& other) const;
  bool operator!=(const Rational& other) const;
  bool operator<(const Rational& other) const;

  friend Rational operator+(Rational lhs, const Rational& rhs);
  friend Rational operator-(Rational lhs, const Rational& rhs);
  friend Rational operator*(Rational lhs, const Rational& rhs);
  friend Rational operator/(Rational lhs, const Rational& rhs);

  friend std::ostream& operator<<(std::ostream& os, const Rational& r);

 private:
  void Normalize();
  bool IsZero() const;

  int64_t numerator_ = 0;
  int64_t denominator_ = 1;
};
}  // namespace groebner
