#ifndef gnat_gfx_math_quaternion_H
#define gnat_gfx_math_quaternion_H

#include "gnat.h"

namespace gnat {

class Vector3;
class Matrix3;
class Matrix4;

class Quaternion {
 public:
  Real x, y, z, w;

  Quaternion(Real x_in, Real y_in, Real z_in, Real w_in)
      : x(x_in), y(y_in), z(z_in), w(w_in) {}

  static const Quaternion IDENTITY;
  static const Quaternion ZERO;

  Quaternion(const Matrix3& mat) { this->FromRotationMatrix(mat); }

  /** Performs spherical linear interpolation between two Quaternions
  * 		@param t The interpolation factor
  * 		@param p The first Quaternion
  * 		@param q The second Quaternion
  * 		@param shortest_path Whether or not to use the shortest path */
  static Quaternion Slerp(Real t,
                          const Quaternion& p,
                          const Quaternion& q,
                          bool shortest_path = true);

  /** Dots this Quaternion with another
  * 		@param q The Quaternion to dot with
  * 		@return The dot product */
  Real DotProduct(const Quaternion& q) const;

  /** Normalizes the Quaternion
  * 		@return The original length */
  Real Normalize(void);

  /** Constructs a quaternion from an angle and an axis
  * 		@param angle The angle
  * 		@param axis The axis */
  void FromAngleAxis(const Real& angle, const Vector3& axis);

  /** Constructs a quaternion from a 3x3 matrix
  * 		@param rot The matrix to use */
  void FromRotationMatrix(const Matrix3& rot);

  /** Returns a matrix representation of this rotation
  * 		@param rot The matrix to output to */
  void ToRotationMatrix(Matrix3& rot) const;

  void FromAxes(const Vector3& xA, const Vector3& yA, const Vector3& zA);

  /** Computes and returns the inverse of this Quaternion
  * 		@returns The inverse Quaternion */
  Quaternion Inverse() const;

  Vector3 operator*(Vector3 vect);
  Quaternion operator-(const Quaternion& q) const;
  Quaternion operator+(const Quaternion& q) const;
  Quaternion operator*(const Quaternion& q) const;
  Quaternion operator*(Real scalar) const;
  Quaternion operator-() const;
  friend Quaternion operator*(Real scalar, const Quaternion& q);
};

}  // namespace gnat

#endif  // gnat_gfx_math_quaternion_H
