cmake_minimum_required (VERSION 3.8)

project ("calc_dlls")

set(CMAKE_CXX_STANDARD 17)

add_library(sin_cos SHARED "sin_cos.cpp" "include/operation.h" "include/variable.h" "include/token.h")
add_library(base SHARED "base.cpp" "include/operation.h" "include/variable.h" "include/token.h")
add_library(pow SHARED "pow.cpp" "include/operation.h" "include/variable.h" "include/token.h")
