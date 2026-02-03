cmake_minimum_required(VERSION 3.12)
project(Groebnerbases VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR})

set(HEADERS
    monomial/monomial.h
    monomial/term.h
    monomial/orders.h
    monomial/comparator.h
    monomial/polynomial.h
    algorithms/division.h
)

set(SOURCES
    main.cpp
    monomial/monomial.cpp
    monomial/term.cpp
    monomial/polynomial.cpp
    algorithms/division.cpp
)

add_executable(Groebnerbases ${SOURCES} ${HEADERS})

