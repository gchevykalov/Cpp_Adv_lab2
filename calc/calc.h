#pragma once

#include "include/operation.h"
#include "include/variable.h"

/**
 * @brief Class of the rpn queue evaluator
 */
class calculator_t {
private:
  ops_maps ops;        ///< Struct which stores all known operations
public:
  /**
   * Default constructor
   */
  calculator_t() = default;

  /**
   * Set table of operations and functions
   * @param[in] opepations - stract with operations
   */
  void setOperations(ops_maps const& operations) {
    ops = operations;
  }

  /**
   * Calculate by rpn queue
   * @param[in] rpnTokens - rpn queue
   * @returns result of calculation
   */
  double calculate(token_queue_t& rpnTokens);

  /**
   * Destructor
   */
  ~calculator_t() = default;
};