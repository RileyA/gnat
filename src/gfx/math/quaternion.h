#ifndef gnat_gfx_math_quaternion_H
#define gnat_gfx_math_quaternion_H

#include "gnat.h"

namespace gnat {

class Quaternion {
 public:
  Real x,y,z,w;

  Quaternion(Real x_in, Real y_in, Real z_in, Real w_in)
    :x(x_in), y(y_in), z(z_in), w(w_in) {}

  static const Quaternion IDENTITY;
  static const Quaternion ZERO;
  
};

}  // namespace gnat

#endif  // gnat_gfx_math_quaternion_H
