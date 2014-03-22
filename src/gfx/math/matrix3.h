#ifndef gnat_gfx_math_matrix3_H
#define gnat_gfx_math_matrix3_H

#include "gnat.h"
#include "gfx/math/vector3.h"

namespace gnat {

class Matrix3 {
 public:

  Matrix3() {};

  explicit Matrix3(const float arr[3][3]) { memcpy(m, arr, 9 * sizeof(float)); }

  Matrix3(const Matrix3& matrix) { memcpy(m, matrix.m, 9 * sizeof(float)); }

  Matrix3(Real entry00,
          Real entry01,
          Real entry02,
          Real entry10,
          Real entry11,
          Real entry12,
          Real entry20,
          Real entry21,
          Real entry22) {
    m[0][0] = entry00;
    m[0][1] = entry01;
    m[0][2] = entry02;
    m[1][0] = entry10;
    m[1][1] = entry11;
    m[1][2] = entry12;
    m[2][0] = entry20;
    m[2][1] = entry21;
    m[2][2] = entry22;
  }
  ;

  void swap(Matrix3& other) {
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

  float* operator[](size_t row) const { return (float*)m[row]; }

  void SetColumn(size_t col, const Vector3& vec);

  Vector3 operator*(const Vector3& vector) const;

  friend Vector3 operator*(const Vector3& vector, const Matrix3& matrix);

  Matrix3 operator*(Real scalar) const;

  friend Matrix3 operator*(Real scalar, const Matrix3& matrix);

  void FromAxes(const Vector3& x_axis,
                const Vector3& y_axis,
                const Vector3& z_axis);

  bool Inverse(Matrix3& inverse, Real tolerance = 1e-06) const;
  Matrix3 Inverse(Real tolerance = 1e-06) const;

  Matrix3 Transpose() const;

  static const Matrix3 ZERO;
  static const Matrix3 IDENTITY;

  Real m[3][3];
};
}

#endif  // gnat_gfx_math_matrix3_H
