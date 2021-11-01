#pragma once
#include <string>
#include <queue>
#include <stack>
#include <memory>

/**
 * @brief Base token class
 * @see token_number_t, token_operation_t, token_variable_t
 */
struct token_t {
  /**
   * @brief Possible types of token
   */
  enum class token_type_t {
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_OPERATION,
    TOKEN_TYPE_VARIABLE
  };

  token_type_t type;      ///< type of current token
};

/**
 * @brief Queue of pointer on tokens
 */
using token_queue_t = std::queue<std::unique_ptr<token_t>>;

/**
 * @brief Stack of pointer on tokens
 */
using token_stack_t = std::stack<std::unique_ptr<token_t>>;

/**
 * @brief Number token class
 */
struct token_number_t : public token_t {
  double value; ///< value of this number

  /**
   * Constructor
   * param[in] val - the value to be assigned
   */
  token_number_t(double val) noexcept {
    value = val;
    type = token_type_t::TOKEN_TYPE_NUMBER;
  }
};

// Ñlass that implements the operation
class operation_t;

/**
 * @brief Operation token class
 * @see operation_t
 */
struct token_operation_t : public token_t {
  std::shared_ptr<operation_t> operation; ///< operation that token represents

  /**
   * Constructor
   * param[in] op - operation stored in the token
   */
  token_operation_t(std::shared_ptr<operation_t> opp) noexcept {
    operation = opp;
    type = token_type_t::TOKEN_TYPE_OPERATION;
  }
};

// Ñlass that implements the variable
class variable_t;

/**
 * @brief Variable token class
 * @see variable_t
 */
struct token_variable_t : public token_t {
  std::shared_ptr<variable_t> var;  ///< variable that token represents

  /**
   * Constructor
   * param[in] varp - variable stored in the token
   */
  token_variable_t(std::shared_ptr<variable_t> varp) noexcept {
    var = varp;
    type = token_type_t::TOKEN_TYPE_VARIABLE;
  }
};