#ifndef gnat_gfx_math_vector4_H
#define gnat_gfx_math_vector4_H

#include "gnat.h"
#include "event/event_handler.h"
#include "util/color.h"

#include "gfx/math/simd/vector4_simd.h"

//#define USE_VECTOR4_SIMD

namespace gnat {

/** A 4d vector */
class Vector4 {
 public:
  Real x, y, z, w;

  Vector4(Real x_in, Real y_in, Real z_in, Real w_in)
    : x(x_in), y(y_in), z(z_in), w(w_in) {}

  Vector4()
    : x(0.0), y(0.0), z(0.0), w(0.0) {}

#ifdef USE_VECTOR4_SIMD
  Vector4 operator +(const Vector4& v) const;
#else
  Vector4 operator +(const Vector4& v) const {
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
  }
#endif

  bool operator ==(const Vector4& v) const {
    return x == v.x && y == v.y && z == v.z && w == v.w;
  }

  static const Vector4 ZERO;
  static const Vector4 UNIT_X;
  static const Vector4 UNIT_Y;
  static const Vector4 UNIT_Z;
  static const Vector4 UNIT_SCALE;
  static const Vector4 NEGATIVE_UNIT_X;
  static const Vector4 NEGATIVE_UNIT_Y;
  static const Vector4 NEGATIVE_UNIT_Z;
};

}  // namespace gnat

#endif  // gnat_gfx_camera_H
