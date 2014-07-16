#ifndef gnat_gfx_math_simd_util_H
#define gnat_gfx_math_simd_util_H

#include "gnat.h"

namespace gnat {
  void EmptyRegisterStateIntrinsic() { _mm_empty(); }
}

#endif  // gnat_gfx_math_simd_H
