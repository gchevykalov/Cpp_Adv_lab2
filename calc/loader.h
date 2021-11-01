#pragma once

#include <vector>
#include <filesystem>
#include <windows.h>
#include "include/operation.h"
#include "include/variable.h"

/**
 * The type of pointer to a function that adds elements from a dll
 */
using dllfuncp = void (*)(ops_maps&, cv_map&);

/**
 * Class that loads elements from dlls
 */
class loader_t {
private:
  std::vector<HMODULE> loadedDlls;   ///< loaded dlls we want to free after session
public:
  ops_maps loadedOps;                ///< operators and function loaded from all plugins 
  cv_map cv;                         ///< const value (like pi or e) loaded from all plugins

  /**
   * Constructor from path to directory
   * @param[in] path - relative path to plugins directory. Default "plugins".
   */
  loader_t(std::string path = "plugins");

  /**
   * Method that checks the loaded elements for compatibility
   * @return true if the loaded items are compatible
   */
  bool checkLoadedElems();

  /**
   * Destructor
   */
  ~loader_t();
};