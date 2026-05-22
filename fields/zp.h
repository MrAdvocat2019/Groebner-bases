#pragma once
#include <cassert>
#include <cstdint>
#include <ostream>

namespace groebner {
template <int64_t P>
class Zp {
 public:
  Zp() = default;
  Zp(int64_t num) : num_(num) { Normalize(); }

  Zp operator-() const { return Zp(-num_); }

  Zp& operator+=(const Zp& other) {
    num_ += other.num_;
    Normalize();
    return *this;
  }
  Zp& operator-=(const Zp& other) {
    num_ -= other.num_;
    Normalize();
    return *this;
  }
  Zp& operator*=(const Zp& other) {
    num_ *= other.num_;
    Normalize();
    return *this;
  }

  Zp& operator/=(const Zp& other) {
    assert(!other.IsZero());
    num_ = num_ * PowMod(other.num_, P - 2) % P;
    Normalize();
    return *this;
  }

  bool operator==(const Zp& other) const { return num_ == other.num_; }
  bool operator!=(const Zp& other) const { return num_ != other.num_; }

  friend Zp operator+(Zp lhs, const Zp& rhs) {
    lhs += rhs;
    return lhs;
  }
  friend Zp operator-(Zp lhs, const Zp& rhs) {
    lhs -= rhs;
    return lhs;
  }
  friend Zp operator*(Zp lhs, const Zp& rhs) {
    lhs *= rhs;
    return lhs;
  }
  friend Zp operator/(Zp lhs, const Zp& rhs) {
    lhs /= rhs;
    return lhs;
  }

  friend std::ostream& operator<<(std::ostream& os, const Zp& z) {
    return os << z.num_;
  }

 private:
  void Normalize() {
    num_ %= P;
    if (num_ < 0) {
      num_ += P;
    }
  }
  bool IsZero() const { return num_ == 0; }

  static int64_t PowMod(int64_t base, int64_t exp) {
    int64_t result = 1;
    base %= P;
    if (base < 0) base += P;
    while (exp > 0) {
      if (exp & 1) result = result * base % P;
      base = base * base % P;
      exp >>= 1;
    }
    return result;
  }

  int64_t num_ = 0;
};
}  // namespace groebner
