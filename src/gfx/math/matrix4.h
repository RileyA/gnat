#ifndef gnat_gfx_math_matrix4_H
#define gnat_gfx_math_matrix4_H

#include "gnat.h"

#include <cstring>

namespace gnat {

class Matrix4 {
 public:
  void Zero() {
    memset(m, 0, sizeof(m));
  }
  Real m[4][4];
};

}

#endif  // gnat_gfx_math_matrix4_H
