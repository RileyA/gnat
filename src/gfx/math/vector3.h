#ifndef gnat_gfx_math_vector3_H
#define gnat_gfx_math_vector3_H

#include "gnat.h"
#include "event/event_handler.h"
#include "util/color.h"

namespace gnat {

/** A 3d vector */
class Vector3 {
 public:
  Real x, y, z;

  Vector3(Real x_in, Real y_in, Real z_in)
    : x(x_in), y(y_in), z(z_in) {}

  Vector3 operator+(const Vector3& v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
  }

  bool operator==(const Vector3& v) const {
    return x == v.x && y == v.y && z == v.z;
  }

  static const Vector3 ZERO;
  static const Vector3 UNIT_X;
  static const Vector3 UNIT_Y;
  static const Vector3 UNIT_Z;
  static const Vector3 UNIT_SCALE;
  static const Vector3 NEGATIVE_UNIT_X;
  static const Vector3 NEGATIVE_UNIT_Y;
  static const Vector3 NEGATIVE_UNIT_Z;
};

}  // namespace gnat

#endif  // gnat_gfx_camera_H
