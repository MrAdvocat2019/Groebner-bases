#pragma once
#include "monomial.h"

namespace groebner {

class LexOrder{
public:
    bool operator()(const Monomial& lhs, const Monomial& rhs) const {
        return lhs < rhs;
    }
    
    bool operator==(const LexOrder&) const {
        return true; // Все экземпляры LexOrder эквивалентны
    }
};

class GrLexOrder{
public:
    bool operator()(const Monomial& lhs, const Monomial& rhs) const {
        if(lhs.GetDegree() != rhs.GetDegree()) {
            return lhs.GetDegree() < rhs.GetDegree();
        }
        return lhs < rhs;
    }
    
    bool operator==(const GrLexOrder&) const {
        return true; // Все экземпляры GrLexOrder эквивалентны
    }
};
}
