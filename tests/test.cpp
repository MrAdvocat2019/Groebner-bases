#include <iostream>
#include <string>

#include "algorithms/buchberger.h"
#include "fields/rational.h"
#include "monomial/orders.h"
#include "tests/cyclic.h"
#include "tests/timer.h"

int main() {
  for (int i = 1; i <= 5; ++i) {
    {
      auto timer = groebner::testing::ScopedTimer("Building cyclic of size:" +
                                                  std::to_string(i));
      auto cyclic = groebner::testing::MakeCyclic<groebner::Rational>(i);
    }
  }
  for (int i = 1; i <= 5; ++i) {
    {
      auto timer = groebner::testing::ScopedTimer(
          "Building groebner of cyclic of size:" + std::to_string(i));
      auto cyclic = groebner::testing::MakeCyclic<groebner::Rational>(
          i, groebner::Order::GrLex());
      auto groebner = groebner::algorithms::Buchberger(cyclic);
    }
  }
}
