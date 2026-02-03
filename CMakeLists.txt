cmake_minimum_required(VERSION 3.12)
project(Groebnerbases VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR})

set(HEADERS
    monomilal/monomial.h
    monomilal/orders.h
    monomilal/comparator.h
    monomilal/polynomial.h
)

set(SOURCES
    main.cpp
    monomilal/monomial.cpp
    monomilal/polynomial.cpp
)

add_executable(Groebnerbases ${SOURCES} ${HEADERS})

