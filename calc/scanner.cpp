#include "scanner.h"

/**
 * Clear the queue of tokens
 */
void scanner_t::clearQueue() {
  while (!tokens.empty())
    tokens.pop();
}

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
bool scanner_t::processName(std::string const& expression, size_t& index, ops_maps const& ops,
                              cv_map const& cv, vars_map& vars) {
  size_t start = index;
  std::string name;

  while (expression[index] && (isalpha(expression[index]) || expression[index] == '_')) {
    ++index;
  }
  name = expression.substr(start, index - start);

  auto fi = ops.funcs.find(name); // attempt to process as a function name
  if (fi != ops.funcs.end()) {
    tokens.push(std::unique_ptr<token_t>(new token_operation_t(fi->second)));
    return false;
  }

  auto ci = cv.find(name); // attempt to process as a constant name
  if (ci != cv.end()) {
    tokens.push(std::unique_ptr<token_t>(new token_number_t(ci->second)));
    return true;
  }

  auto vi = vars.find(name); // treat as a variable name
  if (vi != vars.end())
    tokens.push(std::unique_ptr<token_t>(new token_variable_t(vi->second)));
  else { // there is no variable with this name
    vars.insert(std::make_pair(name, std::shared_ptr<variable_t>(new variable_t)));
    tokens.push(std::unique_ptr<token_t>(new token_variable_t(vars[name])));
  }
  return true;
}

/**
 * Process designation of operators
 * @param[in] expression - string we want to translate
 * @param[in] index - possition in expression
 * @param[in] ops - operations loaded from plugins
 * @param[in] state - state before processing (true if last token was number, variable or postfix operation)
 * @param[out] index - possition in expression after name processing
 * @return state after processing (true if last token was number, variable or postfix operation)
 */
bool scanner_t::processOperators(std::string const& expression, size_t& index, ops_maps const& ops, bool state) {
  size_t start = index;
  size_t end = index;
  bool isAfterNum = false;

  if (state == false) { // process prefix operation
    std::string err = "Unknown prefix operation: ";

    // finding the longest matching
    while (expression[index] && !isalpha(expression[index]) && !isspace(expression[index])
      && !isdigit(expression[index]) && expression[index] != '.' && expression[index] != '_') {
      ++index;
      if (ops.pref.find(expression.substr(start, index - start)) != ops.pref.end()) {
        end = index;
      }
    }

    if (start == end) { // prefix operation not found
      err += expression.substr(start, index - start);
      throw(std::exception(err.c_str()));
    }
    else { // adding the token
      auto poi = ops.pref.find(expression.substr(start, end - start));
      tokens.push(std::unique_ptr<token_t>(new token_operation_t(poi->second)));
      index = end;
    }
  }
  else { // process postfix or infix operation
    std::string err = "Unknown postfix or infix operation: ";

    // finding the longest matching
    while (expression[index] && !isalpha(expression[index]) && !isspace(expression[index])
      && !isdigit(expression[index]) && expression[index] != '.' && expression[index] != '_') {
      ++index;
      if (ops.postf.find(expression.substr(start, index - start)) != ops.postf.end()) {
        end = index;
        isAfterNum = true;
        continue;
      }
      if (ops.inf.find(expression.substr(start, index - start)) != ops.inf.end()) {
        end = index;
        isAfterNum = false;
      }
    }

    if (start == end) { // infix or postfix operation not found
      err += expression.substr(start, index - start);
      throw(std::exception(err.c_str()));
    }
    else {
      if (isAfterNum) { // adding the token with postfix operation
        auto poi = ops.postf.find(expression.substr(start, end - start));
        tokens.push(std::unique_ptr<token_t>(new token_operation_t(poi->second)));
      }
      else { // adding the token with infix operation
        auto ioi = ops.inf.find(expression.substr(start, end - start));
        tokens.push(std::unique_ptr<token_t>(new token_operation_t(ioi->second)));
      }
      index = end;
    }
  }

  return isAfterNum;
}

/**
 * Scan string and translate to token_queue_t
 * @param[in] expression - string we want to translate
 * @param[in] ops - operations loaded from plugins
 * @param[in] cv - named const values loaded from plugins
 * @param[in] vars - map of variables
 * @param[out] vars - augmented map of variables
 * @return queue of tokens
 */
token_queue_t& scanner_t::scan(std::string const& expression, ops_maps const& ops,
                                 cv_map const& cv, vars_map& vars) {
  if (tokens.size() != 0)
    clearQueue();

  size_t index = 0;
  bool isAfterNum = false;
  while (index < expression.size()) {
    if (isspace(expression[index])) { // skip spaces
      index++;
      continue;
    }
    else if (expression[index] == '.' || isdigit(expression[index])) { // process a number
      double val = 0;
      std::string::size_type shift;
      val = std::stod(expression.substr(index), &shift);
      tokens.push(std::unique_ptr<token_t>(new token_number_t(val)));
      index += shift;
      isAfterNum = true;
    }
    else if (expression[index] == '_' || isalpha(expression[index])) { // process a name
      isAfterNum = processName(expression, index, ops, cv, vars);
    }
    else {
      isAfterNum = processOperators(expression, index, ops, isAfterNum); // process a designation
    }
  }

  return tokens;
}