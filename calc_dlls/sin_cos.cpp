#include "include/operation.h"
#include <cmath>

class Cosinus : public function_t {
public:
  Cosinus() = default;

  ~Cosinus() = default;

  void process(token_stack_t& stack) override {
    double operand = getNumber(stack);

    stack.push(std::unique_ptr<token_number_t>(new token_number_t(cos(operand))));
  }
};

class Sinus : public function_t {
public:
  Sinus() = default;

  ~Sinus() = default;

  void process(token_stack_t& stack) override {
    double operand = getNumber(stack);

    stack.push(std::unique_ptr<token_number_t>(new token_number_t(sin(operand))));
  }
};

extern "C" __declspec(dllexport) void __cdecl load(ops_maps& m, std::map<std::string, double const>& cv) {
  m.funcs.insert(std::make_pair("cos", std::shared_ptr<function_t>(new Cosinus)));
  m.funcs.insert(std::make_pair("sin", std::shared_ptr<function_t>(new Sinus)));
  cv.insert(std::make_pair("pi", 3.1415926535897932384626433832795));
}