cmake_minimum_required(VERSION 3.12)
project(Groebnerbases VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include_directories(${CMAKE_SOURCE_DIR})


set(SOURCES
    main.cpp
    monomial/monomial.cpp
    monomial/orders.cpp
    fields/rational.cpp
)

add_executable(Groebnerbases ${SOURCES})
