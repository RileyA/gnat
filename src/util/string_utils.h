#ifndef gnat_util_string_utils_H
#define gnat_util_string_utils_H

#include "gnat.h"
#include <sstream>

namespace gnat {

template<typename T>
String to_str(T val) {
  std::stringstream ss;
  ss << val;
  return ss.str();
}

}  // namespace gnat

#endif  // gnat_util_string_utils_H
