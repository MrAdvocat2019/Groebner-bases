#include "rational.h"

#include <cassert>
#include <cstdint>
#include <numeric>
#include <ostream>

namespace groebner {
Rational::Rational(int64_t nominator, int64_t denominator)
    : numerator_(nominator), denominator_(denominator) {
  assert(denominator != 0);
  Normalize();
}
Rational::Rational(int64_t num) : numerator_(num), denominator_(1) {}

int64_t Rational::Numerator() const { return numerator_; }

int64_t Rational::Denominator() const { return denominator_; }

Rational Rational::operator-() const {
  return Rational(-numerator_, denominator_);
}

Rational& Rational::operator+=(const Rational& other) {
  int64_t new_denominator = std::lcm(denominator_, other.denominator_);
  int64_t new_numerator =
      numerator_ * (new_denominator / denominator_) +
      other.numerator_ * (new_denominator / other.denominator_);
  numerator_ = new_numerator;
  denominator_ = new_denominator;
  Normalize();
  return *this;
}

Rational& Rational::operator-=(const Rational& other) {
  int64_t new_denominator = std::lcm(denominator_, other.denominator_);
  int64_t new_numerator =
      numerator_ * (new_denominator / denominator_) -
      other.numerator_ * (new_denominator / other.denominator_);
  numerator_ = new_numerator;
  denominator_ = new_denominator;
  Normalize();
  return *this;
}

Rational& Rational::operator*=(const Rational& other) {
  int64_t new_denominator = denominator_ * other.denominator_;
  int64_t new_numerator = numerator_ * other.numerator_;
  numerator_ = new_numerator;
  denominator_ = new_denominator;
  Normalize();
  return *this;
}

Rational& Rational::operator/=(const Rational& other) {
  assert(!other.IsZero());
  int64_t new_denominator = denominator_ * other.numerator_;
  int64_t new_numerator = numerator_ * other.denominator_;
  numerator_ = new_numerator;
  denominator_ = new_denominator;
  Normalize();
  return *this;
}

bool Rational::operator==(const Rational& other) const {
  return numerator_ == other.numerator_ && denominator_ == other.denominator_;
}

bool Rational::operator!=(const Rational& other) const {
  return numerator_ != other.numerator_ || denominator_ != other.denominator_;
}

bool Rational::operator<(const Rational& other) const {
  return numerator_ * other.denominator_ < denominator_ * other.numerator_;
}

bool Rational::IsZero() const { return numerator_ == 0; }

void Rational::Normalize() {
  assert(denominator_ != 0);
  if (denominator_ < 0) {
    numerator_ = -numerator_;
    denominator_ = -denominator_;
  }
  int64_t g = std::gcd(numerator_, denominator_);
  numerator_ /= g;
  denominator_ /= g;
}

Rational operator+(Rational lhs, const Rational& rhs) {
  lhs += rhs;
  return lhs;
}

Rational operator-(Rational lhs, const Rational& rhs) {
  lhs -= rhs;
  return lhs;
}

Rational operator*(Rational lhs, const Rational& rhs) {
  lhs *= rhs;
  return lhs;
}

Rational operator/(Rational lhs, const Rational& rhs) {
  lhs /= rhs;
  return lhs;
}

std::ostream& operator<<(std::ostream& os, const Rational& r) {
  if (r.denominator_ == 1) {
    return os << r.numerator_;
  }
  return os << r.numerator_ << "/" << r.denominator_;
}

}  // namespace groebner
