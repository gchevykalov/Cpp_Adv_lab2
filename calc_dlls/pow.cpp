#include "include/operation.h"
#include <cmath>

class Pow : public infix_t {
public:
  Pow() : infix_t(5.0, infix_t::operation_assoc_t::TO_LEFT) {}

  ~Pow() = default;

  void process(token_stack_t& stack) override {
    double power = getNumber(stack);
    double operand = getNumber(stack);

    if (operand < 0 && power < 0)
      throw std::exception("Incorrect operand in ^");

    stack.push(std::unique_ptr<token_number_t>(new token_number_t(pow(operand, power))));
  }
};

extern "C" __declspec(dllexport) void __cdecl load(ops_maps & m, std::map<std::string, double const>&cv) {
  m.inf.insert(std::make_pair("^", std::shared_ptr<infix_t>(new Pow)));
}