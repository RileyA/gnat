#include "gnat.h"

#include "gfx/math/vector3.h"
#include "gfx/math/quaternion.h"

namespace gnat {

const Vector3 Vector3::ZERO(0.0, 0.0, 0.0);
const Vector3 Vector3::UNIT_X(1.0, 0.0, 0.0);
const Vector3 Vector3::UNIT_Y(0.0, 1.0, 0.0);
const Vector3 Vector3::UNIT_Z(0.0, 0.0, 1.0);
const Vector3 Vector3::NEGATIVE_UNIT_X(-1.0, 0.0, 0.0);
const Vector3 Vector3::NEGATIVE_UNIT_Y(0.0, -1.0, 0.0);
const Vector3 Vector3::NEGATIVE_UNIT_Z(0.0, 0.0, -1.0);
const Vector3 Vector3::UNIT_SCALE(1.0, 1.0, 1.0);

Quaternion Vector3::GetRotationTo(const Vector3& dest,
                                  const Vector3& fallback) const {
  // Based on Stan Melax's article in Game Programming Gems
  Vector3 fallback_axis = fallback;
  Quaternion q(0, 0, 0, 0);
  Vector3 v0 = *this;
  Vector3 v1 = dest;
  v0.Normalize();
  v1.Normalize();

  Real d = v0.DotProduct(v1);

  // If dot == 1, vectors are the same
  if (d >= 1.0f) {
    return Quaternion::IDENTITY;
  }

  if (d < (1e-6f - 1.0f)) {
    if (fallback_axis != Vector3::ZERO) {
      // rotate 180 degrees about the fallback axis
      q.FromAngleAxis(3.141592, fallback_axis);
    } else {
      // Generate an axis
      Vector3 axis = Vector3::UNIT_X.CrossProduct(*this);

      if (axis.IsZeroLength()) {  // pick another if colinear
        axis = Vector3::UNIT_Y.CrossProduct(*this);
        axis.Normalize();
        q.FromAngleAxis(3.141592, axis);
      }
    }
  } else {
    Real s = sqrt((1 + d) * 2);
    Real invs = 1 / s;
    Vector3 c = v0.CrossProduct(v1);
    q.x = c.x * invs;
    q.y = c.y * invs;
    q.z = c.z * invs;
    q.w = s * 0.5f;
    q.Normalize();
  }
  return q;
}
//-----------------------------------------------------------------------

Vector3 Vector3::RandomDeviant(const Real& angle, const Vector3& up) const {
  NOTREACHED();
  return up;
  /*Vector3 newUp(0,0,0);
  newUp = up;

  // Rotate up vector by random amount around this
  Quaternion q(0,0,0,0);
  q.FromAngleAxis(3.1415926 * 2.f * Rand::get().genFloat(0.f, 1.f), *this );
  newUp = q * newUp;

  // Finally rotate this by given angle around randomised up
  q.FromAngleAxis( angle, newUp );
  return q * (*this);*/
}
//-----------------------------------------------------------------------

}  // namespace gnat
