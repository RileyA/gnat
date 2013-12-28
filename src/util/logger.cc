#include "util/logger.h"

#include "gnat.h"
#include <iostream>

namespace gnat {

Logger::Logger() {}

Logger::~Logger() {}

void Logger::log(String message, int level) {
  std::cout << message << std::endl;
}

}

