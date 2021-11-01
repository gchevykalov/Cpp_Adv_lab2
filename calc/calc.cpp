#include "calc.h"

/**
 * Calculate by rpn queue
 * @param[in] rpnTokens - rpn queue
 * @returns result of calculation
 */
double calculator_t::calculate(token_queue_t& rpnTokens) {
  token_stack_t operands;

  while (!rpnTokens.empty()) {
    std::unique_ptr<token_t> tok = std::move(rpnTokens.front());
    rpnTokens.pop();

    if (tok->type == token_t::token_type_t::TOKEN_TYPE_NUMBER ||
          tok->type == token_t::token_type_t::TOKEN_TYPE_VARIABLE) {
      operands.push(std::move(tok));
    }
    else {
      token_operation_t* op = static_cast<token_operation_t*>(tok.get());

      op->operation->process(operands);
    }
  }

  if (operands.size() != 1)
    throw std::exception("Syntax error");

  std::unique_ptr<token_t> tok = std::move(operands.top());
  operands.pop();

  if (tok->type != token_t::token_type_t::TOKEN_TYPE_NUMBER)
    throw std::exception("Unexpected type of result");

  token_number_t* num = static_cast<token_number_t*>(tok.get());
  
  return num->value;
}