#pragma once

#include "include/variable.h"
#include "include/operation.h"

/**
 * @brief Class that splits an expression string into tokens
 */
class scanner_t {
private:
  token_queue_t tokens; ///< resulting queue of tokens

  /**
   * Process name of function, named const value or variables
   * @param[in] expression - string we want to translate
   * @param[in] index - possition in expression
   * @param[in] ops - operations loaded from plugins
   * @param[in] cv - named const values loaded from plugins
   * @param[in] vars - map of variables
   * @param[out] vars - augmented map of variables
   * @param[out] index - possition in expression after name processing
   * @return state after processing (true if last token was number, variable or postfix operation)
   */
  bool processName(std::string const& expression, size_t& index, ops_maps const& ops,
                     cv_map const& cv, vars_map& vars);

  /**
   * Process designation of operators
   * @param[in] expression - string we want to translate
   * @param[in] index - possition in expression
   * @param[in] ops - operations loaded from plugins
   * @param[in] state - state before processing (true if last token was number, variable or postfix operation)
   * @param[out] index - possition in expression after name processing
   * @return state after processing (true if last token was number, variable or postfix operation)
   */
  bool processOperators(std::string const& expression, size_t& index, ops_maps const& ops, bool state);

  /**
   * Clear the queue of tokens
   */
  void clearQueue();

public:
  /**
   * Default constructor
   */
  scanner_t() = default;

  /**
   * Scan string and translate to token_queue_t
   * @param[in] expression - string we want to translate
   * @param[in] ops - operations loaded from plugins
   * @param[in] cv - named const values loaded from plugins
   * @param[in] vars - map of variables
   * @param[out] vars - augmented map of variables
   * @return queue of tokens
   */
  token_queue_t& scan(std::string const& expression, ops_maps const& ops, cv_map const& cv, vars_map& vars);

  /**
   * Destructor
   */
  ~scanner_t() = default;
};