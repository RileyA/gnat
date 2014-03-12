#ifndef gnat_util_color_H
#define gnat_util_color_H

#include "gnat.h"

namespace gnat {

template<typename T>
class Color {
 public:
  T r,g,b,a;
  Color() {}
  Color(T r_in, T g_in, T b_in) 
    : r(r_in),
      g(g_in),
      b(b_in) {}
  Color(T r_in, T g_in, T b_in, T a_in)
    : r(r_in),
      g(g_in),
      b(b_in),
      a(a_in) {}

  const T& operator[](size_t idx) const {
    DCHECK(idx < 4);
    return *((&r) + idx);
  }

  T& operator[](size_t idx) {
    DCHECK(idx < 4);
    return *((&r) + idx);
  }
};

typedef Color<Byte> ColorU8;
typedef Color<float> ColorF32;
}  // namespace gnat

#endif  // gnat_util_color_H
