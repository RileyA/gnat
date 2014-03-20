#ifndef gnat_gfx_math_quaternion_H
#define gnat_gfx_math_quaternion_H

#include "gnat.h"

namespace gnat {

class Vector3;
class Matrix3;
class Matrix4;

class Quaternion {
 public:
  Real x,y,z,w;

  Quaternion(Real x_in, Real y_in, Real z_in, Real w_in)
    :x(x_in), y(y_in), z(z_in), w(w_in) {}

  static const Quaternion IDENTITY;
  static const Quaternion ZERO;
  
  Quaternion(const Matrix3& mat) {
    this->FromRotationMatrix(mat);
  }

  /** Performs spherical linear interpolation between two Quaternions 
   *		@param fT The interpolation factor 
   *		@param rkP The first Quaternion 
   *		@param rkQ The second Quaternion
   *		@param shortestPath Whether or not to use the shortest path */
  static Quaternion Slerp(Real fT, const Quaternion& rkP,
    const Quaternion& rkQ, bool shortestPath = true);

  /** Dots this Quaternion with another
   *		@param rkQ The Quaternion to dot with 
   *		@return The dot product */
  Real DotProduct(const Quaternion& rkQ) const;

  /** Normalizes the Quaternion
   *		@return The original length */
  Real Normalize(void); 

  /** Constructs a quaternion from an angle and an axis
   *		@param angle The angle
   *		@param kAxis The axis */
  void FromAngleAxis(const Real& angle,const Vector3& kAxis);

  /** Constructs a quaternion from a 3x3 matrix
   *		@param kRot The matrix to use */
  void FromRotationMatrix(const Matrix3& kRot);

  /** Returns a matrix representation of this rotation
   *		@param kRot The matrix to output to */
  void ToRotationMatrix(Matrix3& kRot) const;

  void FromAxes(const Vector3& xA, const Vector3& yA, const Vector3& zA);

  /** Computes and returns the inverse of this Quaternion 
   *		@returns The inverse Quaternion */
  Quaternion Inverse() const;

  Vector3 operator*(Vector3 vect);
  Quaternion operator-(const Quaternion& rkQ) const;
  Quaternion operator+(const Quaternion& rkQ) const;
  Quaternion operator*(const Quaternion& rkQ) const;
  Quaternion operator*(Real fScalar) const;
  Quaternion operator-() const;
  friend Quaternion operator*(Real fScalar, const Quaternion& rkQ);
};

}  // namespace gnat

#endif  // gnat_gfx_math_quaternion_H
