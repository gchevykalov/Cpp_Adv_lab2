#include "loader.h"

/**
 * Constructor from path to directory
 * @param[in] path - relative path to plugins directory. Default "plugins".
 */
loader_t::loader_t(std::string path) {
  HMODULE hdll = NULL;

  for (auto& dll : std::filesystem::directory_iterator(std::filesystem::current_path().string() + "\\" + path)) {
    if (dll.path().extension() == ".dll") {
      hdll = LoadLibrary(dll.path().string().c_str());
      if (hdll) {
        loadedDlls.push_back(hdll);

        dllfuncp load = (dllfuncp)GetProcAddress(hdll, "load");
        if (load)
          load(loadedOps, cv);
      }
    }
  }
}

/**
 * Method that checks the loaded elements for compatibility
 * @return true if the loaded items are compatible
 */
bool loader_t::checkLoadedElems() {
  auto fi = loadedOps.funcs.begin();
  auto ci = cv.begin();
  
  // checking function and constant names
  while (fi != loadedOps.funcs.end() && ci != cv.end()) {
    if (fi->first > ci->first)
      ++ci;
    else if (fi->first < ci->first)
      ++fi;
    else // matching function and constant names
      return false;
  }

  auto infi = loadedOps.inf.begin();
  auto postfi = loadedOps.postf.begin();

  // checking designations of postfix and infix operations
  while (infi != loadedOps.inf.end() && postfi != loadedOps.postf.end()) {
    if (infi->first > postfi->first)
      ++postfi;
    else if (infi->first < postfi->first)
      ++infi;
    else // matching designations of postfix and infix operations
      return false;
  }

  return true;
}

/**
 * Destructor
 */
loader_t::~loader_t() {
  loadedOps.funcs.clear();
  loadedOps.inf.clear();
  loadedOps.pref.clear();
  loadedOps.postf.clear();

  for (auto& dll : loadedDlls)
    FreeLibrary(dll);
}