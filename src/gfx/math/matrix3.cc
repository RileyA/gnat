#include "gfx/math/matrix3.h"
#include "gfx/math/matrix4.h"
#include "gfx/math/quaternion.h"

namespace gnat {

const Matrix3 Matrix3::ZERO(0,0,0,0,0,0,0,0,0);
const Matrix3 Matrix3::IDENTITY(1,0,0,0,1,0,0,0,1);

//-----------------------------------------------------------------------
void Matrix3::SetColumn(size_t col, const Vector3& vec) {
  //assert( 0 <= col && col < 3 );
  m[0][col] = vec.x;
  m[1][col] = vec.y;
  m[2][col] = vec.z;
}
//-----------------------------------------------------------------------

void Matrix3::FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis) {
  SetColumn(0,xAxis);
  SetColumn(1,yAxis);
  SetColumn(2,zAxis);
}

//-----------------------------------------------------------------------

Vector3 Matrix3::operator* (const Vector3& point) const {
  Vector3 prod(0,0,0);
  for (size_t row = 0; row < 3; row++) {
    prod[row] =
      m[row][0]*point[0] +
      m[row][1]*point[1] +
      m[row][2]*point[2];
  }
  return prod;
}
//-----------------------------------------------------------------------

Vector3 operator* (const Vector3& point, const Matrix3& matrix) {
  Vector3 prod(0,0,0);
  for (size_t row = 0; row < 3; row++) {
    prod[row] =
      point[0]*matrix.m[0][row] +
      point[1]*matrix.m[1][row] +
      point[2]*matrix.m[2][row];
  }
  return prod;
}
//-----------------------------------------------------------------------

Matrix3 Matrix3::operator* (Real scalar) const {
  Matrix3 prod;
  for (size_t row = 0; row < 3; row++) {
    for (size_t col = 0; col < 3; col++)
      prod[row][col] = scalar*m[row][col];
  }
  return prod;
}
//-----------------------------------------------------------------------

Matrix3 operator* (Real scalar, const Matrix3& matrix) {
  Matrix3 prod;
  for (size_t row = 0; row < 3; row++) {
    for (size_t col = 0; col < 3; col++)
      prod[row][col] = scalar*matrix.m[row][col];
  }
  return prod;
}
//-----------------------------------------------------------------------

Matrix3 Matrix3::Transpose () const {
  Matrix3 transpose;
  for (size_t row = 0; row < 3; row++) {
    for (size_t col = 0; col < 3; col++)
      transpose[row][col] = m[col][row];
  }
  return transpose;
}
//-----------------------------------------------------------------------

Matrix3 Matrix3::Inverse (Real tolerance) const {
  Matrix3 inverse = Matrix3::ZERO;
  Inverse(inverse, tolerance);
  return inverse;
}
//--------------------------------------------------------------------------

bool Matrix3::Inverse (Matrix3& inverse, Real tolerance) const {
  // Invert a 3x3 using cofactors.  This is about 8 times faster than
  // the Numerical Recipes code which uses Gaussian elimination.
  inverse[0][0] = m[1][1]*m[2][2] -
    m[1][2]*m[2][1];
  inverse[0][1] = m[0][2]*m[2][1] -
    m[0][1]*m[2][2];
  inverse[0][2] = m[0][1]*m[1][2] -
    m[0][2]*m[1][1];
  inverse[1][0] = m[1][2]*m[2][0] -
    m[1][0]*m[2][2];
  inverse[1][1] = m[0][0]*m[2][2] -
    m[0][2]*m[2][0];
  inverse[1][2] = m[0][2]*m[1][0] -
    m[0][0]*m[1][2];
  inverse[2][0] = m[1][0]*m[2][1] -
    m[1][1]*m[2][0];
  inverse[2][1] = m[0][1]*m[2][0] -
    m[0][0]*m[2][1];
  inverse[2][2] = m[0][0]*m[1][1] -
    m[0][1]*m[1][0];

  Real det =
    m[0][0]*inverse[0][0] +
    m[0][1]*inverse[1][0]+
    m[0][2]*inverse[2][0];

  if ( abs(det) <= tolerance )
    return false;

  Real inv_det = 1.0f/det;
  for (size_t row = 0; row < 3; row++) {
    for (size_t col = 0; col < 3; col++)
      inverse[row][col] *= inv_det;
  }

  return true;
}	

}  // namespace gnat
