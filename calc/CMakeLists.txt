cmake_minimum_required (VERSION 3.8)

project ("Calc")

set(CMAKE_CXX_STANDARD 17)

add_executable (Calc "calc.cpp" "calc.h" "include/operation.h" "include/token.h" "include/variable.h" "loader.h" "loader.cpp" "scanner.h" "scanner.cpp" "parser.h" "parser.cpp" "main.cpp" "getResult.h" )