#pragma once

#include <string>
#include <map>
#include "token.h"
#include "variable.h"

/**
 * @brief Base class of operation
 * @warning Among themselves, prefix operations are performed from left to right regardless of priority
 * @warning Among themselves, postfix operations are performed from right to left regardless of priority
 */
class operation_t {
public:
  /**
   * @brief Possible types of operation
   */
  enum class operation_type_t {
    FUNCTION,
    PREFIX_OP,
    INFIX_OP,
    POSTFIX_OP
  };

  operation_type_t const type;  ///< type of operation
  float const prior;            ///< operation priority

  /**
   * Constructor
   * param[in] t - type of operation
   * param[in] p - operation priority
   */
  operation_t(operation_type_t t, float p) : type(t), prior(p) {}

  /**
   * Virtual destructor for the correct destruction of heirs
   */
  virtual ~operation_t() {};

  /**
   * Performs operand processing
   * param[in] stack - stack of operands
   */
  virtual void process(token_stack_t& stack) = 0;

protected:
  /**
   * Extract a number from the operand stack
   * @warning throws std::exception in case of failure
   * param[in] stack - stack of operands
   * @return extracted value
   */
  double getNumber(token_stack_t& stack) {
    std::unique_ptr<token_t> tok = std::move(stack.top());
    stack.pop();

    if (tok->type == token_t::token_type_t::TOKEN_TYPE_OPERATION)
      throw std::exception("Unexpected operation");

    if (tok->type == token_t::token_type_t::TOKEN_TYPE_NUMBER) {
      token_number_t* num = static_cast<token_number_t*>(tok.get());
      return num->value;
    }

    token_variable_t* var = static_cast<token_variable_t*>(tok.get());
    if (var->var.get()->isInit())
      return var->var.get()->getValue();
    else
      throw std::exception("Uninitialized variable");
  }
};

/**
 * @brief Base class of function
 */
class function_t : public operation_t {
public:
  /**
   * Default constructor
   */
  function_t() : operation_t(operation_type_t::FUNCTION, -1.0) {}
};

/**
 * @brief Type of function storage
 */
using funcs_map = std::map<std::string, std::shared_ptr<function_t> const>;

/**
 * @brief Base class of infix operation
 */
class infix_t : public operation_t {
public:
  /**
   * @brief Associativity of operation type
   */
  enum class operation_assoc_t {
    TO_RIGHT,
    TO_LEFT
  };

  operation_assoc_t const assoc;    ///< associativity of operation

  /**
   * Constructor
   * param[in] p - operation priority
   * param[in] a - associativity of operation
   */
  infix_t(float p, operation_assoc_t a) : operation_t(operation_type_t::INFIX_OP, p), assoc(a) {}
};

/**
 * @brief Type of infix operation storage
 */
using inf_map = std::map<std::string, std::shared_ptr<infix_t> const>;

/**
 * @brief Base class of prefix operation
 */
class prefix_op_t : public operation_t {
public:
  /**
   * @brief Possible types of prefix operation
   */
  enum class prefix_type_t {
    PREFIX_OP,
    OPEN_BRACKET
  };

  prefix_type_t const prefixType;  ///< type of prefix operation

  /**
   * Constructor
   * param[in] t - type of prefix operation
   * param[in] p - operation priority
   */
  prefix_op_t(prefix_type_t t, float p) : operation_t(operation_type_t::PREFIX_OP, p), prefixType(t) {}
};

/**
 * @brief Type of prefix operation storage
 */
using pref_map = std::map<std::string, std::shared_ptr<prefix_op_t> const>;

/**
 * @brief Base class for prefix operations proper
 */
class prefix_t : public prefix_op_t {
public:
  /**
   * Constructor
   * param[in] p - operation priority
   */
  prefix_t(float p) : prefix_op_t(prefix_type_t::PREFIX_OP, p) {}
};

/**
 * @brief Base class for opening bracket
 * @warning the id must match both elements of the bracket pair for correct operation
 */
class open_bracket_t : public prefix_op_t {
public:
  int const pairID;  ///< id of the bracket pair

  /**
   * Constructor
   * param[in] id - id of the bracket pair
   */
  open_bracket_t(int id) : prefix_op_t(prefix_type_t::OPEN_BRACKET, -1.0), pairID(id) {}

  /**
   * Does nothing
   */
  void process(token_stack_t& stack) override final {};
};

/**
 * @brief Base class for postfix operations
 */
class postfix_op_t : public operation_t {
public:
  /**
   * @brief Possible types of postfix operation
   */
  enum class postfix_type_t {
    POSTFIX_OP,
    CLOSE_BRACKET
  };

  postfix_type_t const postfixType;  ///< type of postfix operation

  /**
   * Constructor
   * param[in] t - type of postfix operation
   * param[in] p - operation priority
   */
  postfix_op_t(postfix_type_t t, float p) : operation_t(operation_type_t::POSTFIX_OP, p), postfixType(t) {}
};

/**
 * @brief Type of postfix operation storage
 */
using postf_map = std::map<std::string, std::shared_ptr<postfix_op_t> const>;

/**
 * @brief Base class for postfix operations proper
 */
class postfix_t : public postfix_op_t {
public:
  /**
   * Constructor
   * param[in] p - operation priority
   */
  postfix_t(float p) : postfix_op_t(postfix_type_t::POSTFIX_OP, p) {}
};

/**
 * @brief Base class for closing bracket
 * @warning the id must match both elements of the bracket pair for correct operation
 */
class close_bracket_t : public postfix_op_t {
public:
  int const pairID;  ///< id of the bracket pair

  /**
   * Constructor
   * param[in] id - id of the bracket pair
   */
  close_bracket_t(int id) : postfix_op_t(postfix_type_t::CLOSE_BRACKET, -1.0), pairID(id) {}
};

/**
 * @brief Type of all operation storage
 */
struct ops_maps {
  funcs_map funcs;
  inf_map inf;
  pref_map pref;
  postf_map postf;
};