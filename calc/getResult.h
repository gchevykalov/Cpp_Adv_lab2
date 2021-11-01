#pragma once

#include "loader.h"
#include "scanner.h"
#include "parser.h"
#include "calc.h"

/**
 * @brief Class of the string expression evaluator
 */
class str_calc_t {
private:
  loader_t l;             ///< Instance of class which can load operations and functions from plugins
  scanner_t s;            ///< Instance of class which can transform string into queue of tokens
  parser_t p;             ///< Instance of class which can transform queue to Reverse Polish Notation queue
  calculator_t c;         ///< Instance of class which can calculate by Reverse Polish Notation queue
  vars_map v;             ///< Storage of variables created during calculations
  bool dllsIsCompatible;  ///< True if the dll is compatible
public:
  /**
   * Default constuctor
   */
  str_calc_t() {
    dllsIsCompatible = l.checkLoadedElems();
    c.setOperations(l.loadedOps);
  }

  /**
   * Run calculaton from string
   * @warning can throw std::exception if string is incorrect
   * @param[in] expression - string with expression
   * @return result of calculation
   */
  double calculate(std::string const& expression) {
    if (dllsIsCompatible)
      return c.calculate(p.parse(s.scan(expression, l.loadedOps, l.cv, v)));
    else
      throw std::exception("Incompatible plugins");
  }

  /**
   * Destructor
   */
  ~str_calc_t() = default;
};