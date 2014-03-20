#ifndef gnat_gfx_math_matrix3_H
#define gnat_gfx_math_matrix3_H

#include "gnat.h"
#include "gfx/math/vector3.h"

namespace gnat {

class Matrix3
{	
public:

  Matrix3 () {};

  explicit Matrix3 (const float arr[3][3])
  {
    memcpy(m,arr,9*sizeof(float));
  }

  Matrix3 (const Matrix3& rkMatrix)
  {
    memcpy(m,rkMatrix.m,9*sizeof(float));
  }

  Matrix3 (Real fEntry00, Real fEntry01, Real fEntry02,
    Real fEntry10, Real fEntry11, Real fEntry12,
    Real fEntry20, Real fEntry21, Real fEntry22)
  {
    m[0][0] = fEntry00;
    m[0][1] = fEntry01;
    m[0][2] = fEntry02;
    m[1][0] = fEntry10;
    m[1][1] = fEntry11;
    m[1][2] = fEntry12;
    m[2][0] = fEntry20;
    m[2][1] = fEntry21;
    m[2][2] = fEntry22;
  };

  void swap(Matrix3& other)
  {
    std::swap(m[0][0], other.m[0][0]);
    std::swap(m[0][1], other.m[0][1]);
    std::swap(m[0][2], other.m[0][2]);
    std::swap(m[1][0], other.m[1][0]);
    std::swap(m[1][1], other.m[1][1]);
    std::swap(m[1][2], other.m[1][2]);
    std::swap(m[2][0], other.m[2][0]);
    std::swap(m[2][1], other.m[2][1]);
    std::swap(m[2][2], other.m[2][2]);
  }

  float* operator[] (size_t row) const
  {
    return (float*)m[row];
  }

  void SetColumn(size_t iCol, const Vector3& vec);

  Vector3 operator* (const Vector3& rkVector) const;

  friend Vector3 operator* (const Vector3& rkVector,
    const Matrix3& rkMatrix);

  Matrix3 operator* (Real fScalar) const;

  friend Matrix3 operator* (Real fScalar, const Matrix3& rkMatrix);

  void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);

  bool Inverse (Matrix3& rkInverse, Real fTolerance = 1e-06) const;
  Matrix3 Inverse (Real fTolerance = 1e-06) const;

  Matrix3 Transpose() const;

  static const Matrix3 ZERO;
  static const Matrix3 IDENTITY;

  Real m[3][3];
};

}

#endif  // gnat_gfx_math_matrix3_H
