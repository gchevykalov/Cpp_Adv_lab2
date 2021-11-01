#pragma once

#include <string>
#include <map>
#include <memory>

/**
 * @brief Ñlass representing a variable
 */
class variable_t final {
private:
  double value;   ///< value of variable
  bool init;      ///< state of variable: true if was inited

public:
  /**
   * Default constructor
   */
  variable_t() noexcept {
    value = 1.0;
    init = false;
  }

  /**
   * Sets the value of the variable
   * param[in] val - the value to be assigned
   */
  void setValue(double val) noexcept {
    value = val;
    init = true;
  }

  /**
   * Returns the state of the variable
   * @return state of the variable
   */
  bool isInit() const noexcept {
    return init;
  }

  /**
   * Returns the value of the variable
   * @return value of the variable
   */
  double getValue() const noexcept {
    return value;
  }
};

/**
 * @brief Type of variable storage
 */
using vars_map = std::map<std::string, std::shared_ptr<variable_t> const>;
/**
 * @brief Type of named constant storage
 */
using cv_map = std::map<std::string, double const>;