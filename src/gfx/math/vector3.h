#ifndef gnat_gfx_math_vector3_H
#define gnat_gfx_math_vector3_H

#include "gnat.h"
#include "event/event_handler.h"
#include "util/color.h"

namespace gnat {

class Quaternion;

/** A 3d vector */
class Vector3 {
 public:
  Real x, y, z;

  Vector3(Real x_in, Real y_in, Real z_in) : x(x_in), y(y_in), z(z_in) {}

  Vector3() {}

  static const Vector3 ZERO;
  static const Vector3 UNIT_X;
  static const Vector3 UNIT_Y;
  static const Vector3 UNIT_Z;
  static const Vector3 UNIT_SCALE;
  static const Vector3 NEGATIVE_UNIT_X;
  static const Vector3 NEGATIVE_UNIT_Y;
  static const Vector3 NEGATIVE_UNIT_Z;

  /** Tests if this vector's direction equals that of another
  * 		@param vect The vector to test with
  * 		@param tolerance The angle of tolerance (degrees) */
  bool DirectionEquals(const Vector3& vect, const Real& tolerance) {
    Real dot = DotProduct(vect);
    Real angle = acos(dot);
    return abs(angle) <= (3.14159f / 180.f) * tolerance;
  }

  /** Normalize the vector, (make it unit length). Returns the previous length.
   */
  Real Normalize() {
    Real length = sqrt(x * x + y * y + z * z);

    if (length > 1e-08) {
      Real inv_length = 1.0f / length;
      x *= inv_length;
      y *= inv_length;
      z *= inv_length;
    }

    return length;
  }

  /** Gets the length of this vector
  * 		@return The length
  * 		@remarks When possible use SquaredLength(), as this requires a costly
  * 			sqrt() operation. */
  Real Length() const { return sqrt(x * x + y * y + z * z); }

  /** Gets the squared length of this vector
  * 		@return The squared length */
  Real SquaredLength() { return Real(x * x + y * y + z * z); }

  /** Gets the cross product of this vector with another
  * 		@param vect The vector to cross with
  * 		@returns The result of the cross product */
  Vector3 CrossProduct(const Vector3& vect) const {
    return Vector3(y * vect.z - z * vect.y,
                   z * vect.x - x * vect.z,
                   x * vect.y - y * vect.x);
  }

  /** Gets the distance between this vector and another
  * 		@param The vector to compare with
  * 		@return The distance
  * 		@remarks The */
  Real Distance(const Vector3& vect) const { return (*this - vect).Length(); }

  /** Gets the squared distance between this vector and another
  * 		@param vect The vector to get the dist to
  * 		@return The squared distance */
  Real SquaredDistance(const Vector3& vect) const {
    return (*this - vect).Length();
  }

  /** Computes the dot (scalar) product of this vector with another
  * 		@param vect The vector to dot with
  * 		@return The scalar product */
  Real DotProduct(const Vector3 vect) {
    return x * vect.x + y * vect.y + z * vect.z;
  }

  /** Get the angle between this vector and another
  * 		@param dest The vector to get the angle to */
  Real AngleBetween(Vector3 dest) {
    Real len_product = Length() * dest.Length();

    // Divide by zero check
    if (len_product < 1e-6f)
      len_product = 1e-6f;

    Real f = DotProduct(dest) / len_product;

    if (f > 1.f)
      f = 1.f;
    if (f < -1.f)
      f = -1.f;
    return (180.f / 3.14159265f) * (acos(f));
  }

  Real AngleBetween2(Vector3 dest) {
    Real s = CrossProduct(dest).Length();
    Real c = DotProduct(dest);
    return (180.f / 3.14159265f) * atan2(s, c);
  }

  /** Gets whether or not this Vector has a length of zero
  * 		@param Whether it's zero or not */
  bool IsZeroLength() {
    Real sqlen = (x * x) + (y * y) + (z * z);
    return (sqlen < (1e-06 * 1e-06));
  }

  /** Gets the Quaternion required to rotate this vector to another
  * 		@param dest The destination vector
  * 		@return The rotation (as a Quaternion) */
  Quaternion GetRotationTo(const Vector3& dest,
                           const Vector3& fallback = Vector3::ZERO) const;

  bool operator==(const Vector3 vect) const {
    return (x == vect.x && y == vect.y && z == vect.z);
  }

  bool operator!=(const Vector3 vect) {
    return (x != vect.x || y != vect.y || z != vect.z);
  }

  Vector3& operator=(const Vector3 vect) {
    x = vect.x;
    y = vect.y;
    z = vect.z;
    return *this;
  }

  Vector3 operator+(const Vector3 vect) const {
    return Vector3(x + vect.x, y + vect.y, z + vect.z);
  }

  Vector3 operator-(const Vector3 vect) const {
    return Vector3(x - vect.x, y - vect.y, z - vect.z);
  }

  Vector3 operator*(const Vector3 vect) const {
    return Vector3(x * vect.x, y * vect.y, z * vect.z);
  }

  Vector3 operator*(const Real scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
  }

  Vector3 operator/(const Vector3 vect) const {
    return Vector3(x / vect.x, y / vect.y, z / vect.z);
  }

  Vector3 operator/(const Real scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
  }

  Vector3& operator+=(const Vector3 vect) {
    x += vect.x;
    y += vect.y;
    z += vect.z;
    return *this;
  }

  Vector3& operator-=(const Vector3 vect) {
    x -= vect.x;
    y -= vect.y;
    z -= vect.z;
    return *this;
  }

  Vector3& operator*=(const Vector3 vect) {
    x *= vect.x;
    y *= vect.y;
    z *= vect.z;
    return *this;
  }

  Vector3& operator*=(const Real scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  Vector3& operator/=(const Vector3 vect) {
    x /= vect.x;
    y /= vect.y;
    z /= vect.z;
    return *this;
  }

  Vector3& operator/=(const Real scalar) {
    if (scalar == 0) {
      DCHECK(false);
      // TODO: throw an exception here...
      return *this;
    }
    Real invScalar = 1 / scalar;
    x /= invScalar;
    y /= invScalar;
    z /= invScalar;
    return *this;
  }

  Real& operator[](const size_t i) { return *(&x + i); }

  const Real& operator[](const size_t i) const { return *(&x + i); }

  Vector3 RandomDeviant(const Real& angle, const Vector3& up) const;
};

}  // namespace gnat

#endif  // gnat_gfx_camera_H
