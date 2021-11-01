#pragma once

#include "include/variable.h"
#include "include/operation.h"

/**
 * @brief Class which parse queue and transform it to rpn
 */
class parser_t {
private:
  token_queue_t qres;  ///< resulting queue with tokens in rpn
  token_stack_t gen;   ///< general stack with numbers and operations
  token_stack_t oper;  ///< intermediate stack with operation

  /**
   * Possible states of parser
   */
  enum class state_t {
    STATE_OPERAND,
    STATE_OPERATION
  };

  /**
   * Send operators with higher priority from stack with operators to general stack
   * @param[in] op - current token
   */
  void displacementOperations(std::unique_ptr<token_t> op);

  /**
   * Send operators from stack with operators to general stack until any open bracket
   * @return true if any open bracket was found
   */
  bool displacementUntilAnyOpenBracket();

  /**
   * Send operators from stack with operators to general stack until open bracket with the same id
   * @param[in] op - current bracket token
   * @return true if open bracket with the same id was displacement
   */
  bool displacementUntilOpenBracket(std::unique_ptr<token_t> op);

  /**
   * Check which of operators has higher priority
   * @param[in] op1 - first token
   * @param[in] op2 - second token
   * @return true if op1 has higher priority than op2
   */
  bool checkPrior(token_operation_t* op1, token_operation_t* op2);

  /**
   * Processes the current token in the parser state STATE_OPERAND
   * @param[in] op - current token
   * @return state after processing
   */
  state_t processOperand(std::unique_ptr<token_t> op);

  /**
   * Processes the current token in the parser state STATE_OPERATION
   * @param[in] op - current token
   * @return state after processing
   */
  state_t processOperation(std::unique_ptr<token_t> op);

  /**
   * Clear resulting queue, general stack and intermediate stack
   */
  void clear();

public:
  /**
   * Default constructor
   */
  parser_t() = default;

  /**
   * Parse queue and transform it to rpn
   * @param[in] tokens - queue of tokens
   * @return queue of tokens in RPN
   */
  token_queue_t& parse(token_queue_t& tokens);

  /**
   * Destructor
   */
  ~parser_t() = default;
};