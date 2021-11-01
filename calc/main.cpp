#include <iostream>
#include <crtdbg.h>
#include "getResult.h"

int main(int argc, char* argv[]) {
  str_calc_t calc;
  std::string string;

  while (true) {
    size_t i = 0;
    size_t len;

    std::getline(std::cin, string);
    // process until empty line
    if (string.size() == 0) {
      break;
    }

    len = string.length();
    // skip spaces
    for (; i < len; i++) {
      if (isspace(string[i]) == 0) {
        break;
      }
    }
    // only spaces
    if (i == string.length()) {
      std::cout << string << std::endl;
      continue;
    }

    std::cout << string + "  ==  ";

    try {
      std::cout << calc.calculate(string) << std::endl;
    }
    catch (std::exception& e) {
      std::cout << "ERROR: " << e.what() << std::endl;
    }
  }

  _CrtDumpMemoryLeaks();
  return 0;
}