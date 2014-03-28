#include "gfx/math/quaternion.h"
#include "gfx/math/quaternion.h"
#include "gfx/math/matrix3.h"
#include "gfx/math/matrix4.h"

namespace gnat {
const Quaternion Quaternion::IDENTITY(0.0, 0.0, 0.0, 1.0);
const Quaternion Quaternion::ZERO(0.0, 0.0, 0.0, 0.0);

void Quaternion::FromAngleAxis(const Real& angle, const Vector3& axis) {
  float halfAngle(0.5 * angle);
  Real sine = sin(halfAngle);
  w = cos(halfAngle);
  x = sine * axis.x;
  y = sine * axis.y;
  z = sine * axis.z;
}
//-----------------------------------------------------------------------

void Quaternion::FromRotationMatrix(const Matrix3& rot) {
  // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
  // article "Quaternion Calculus and Fast Animation".

  Real trace = rot[0][0] + rot[1][1] + rot[2][2];
  Real root;

  if (trace > 0.0) {
    // |w| > 1/2, may as well choose w > 1/2
    root = sqrt(trace + 1.0f);  // 2w
    w = 0.5f * root;
    root = 0.5f / root;  // 1/(4w)
    x = (rot[2][1] - rot[1][2]) * root;
    y = (rot[0][2] - rot[2][0]) * root;
    z = (rot[1][0] - rot[0][1]) * root;
  } else {
    // |w| <= 1/2
    static size_t s_next[3] = {1, 2, 0};
    size_t i = 0;
    if (rot[1][1] > rot[0][0])
      i = 1;
    if (rot[2][2] > rot[i][i])
      i = 2;
    size_t j = s_next[i];
    size_t k = s_next[j];

    root = sqrt(rot[i][i] - rot[j][j] - rot[k][k] + 1.0f);
    Real* aquat[3] = {&x, &y, &z};
    *aquat[i] = 0.5f * root;
    root = 0.5f / root;
    w = (rot[k][j] - rot[j][k]) * root;
    *aquat[j] = (rot[j][i] + rot[i][j]) * root;
    *aquat[k] = (rot[k][i] + rot[i][k]) * root;
  }
}
//-----------------------------------------------------------------------

void Quaternion::FromAxes(const Vector3& xA,
                          const Vector3& yA,
                          const Vector3& zA) {
  Matrix3 rot;

  rot[0][0] = xA.x;
  rot[1][0] = xA.y;
  rot[2][0] = xA.z;

  rot[0][1] = yA.x;
  rot[1][1] = yA.y;
  rot[2][1] = yA.z;

  rot[0][2] = zA.x;
  rot[1][2] = zA.y;
  rot[2][2] = zA.z;

  FromRotationMatrix(rot);
}

void Quaternion::ToRotationMatrix(Matrix3& rot) const {
  Real tx = x + x;
  Real ty = y + y;
  Real tz = z + z;
  Real twx = tx * w;
  Real twy = ty * w;
  Real twz = tz * w;
  Real txx = tx * x;
  Real txy = ty * x;
  Real txz = tz * x;
  Real tyy = ty * y;
  Real tyz = tz * y;
  Real tzz = tz * z;

  rot[0][0] = 1.0f - (tyy + tzz);
  rot[0][1] = txy - twz;
  rot[0][2] = txz + twy;
  rot[1][0] = txy + twz;
  rot[1][1] = 1.0f - (txx + tzz);
  rot[1][2] = tyz - twx;
  rot[2][0] = txz - twy;
  rot[2][1] = tyz + twx;
  rot[2][2] = 1.0f - (txx + tyy);
}

//-----------------------------------------------------------------------
Real Quaternion::Normalize(void) {
  Real len = w * w + x * x + y * y + z * z;
  Real factor = 1.0f / sqrt(len);
  *this = *this * factor;
  return len;
}
//-----------------------------------------------------------------------

Real Quaternion::DotProduct(const Quaternion& q) const {
  return w * q.w + x * q.x + y * q.y + z * q.z;
}
//-----------------------------------------------------------------------

Quaternion Quaternion::Inverse() const {
  Real norm = w * w + x * x + y * y + z * z;
  if (norm > 0.0) {
    Real inv_norm = 1.0f / norm;
    return Quaternion(
        w * inv_norm, -x * inv_norm, -y * inv_norm, -z * inv_norm);
  } else {
    // return an invalid result to flag the error
    return Quaternion(0, 0, 0, 0);
  }
}
//-----------------------------------------------------------------------

Quaternion Quaternion::Slerp(Real t,
                             const Quaternion& p,
                             const Quaternion& q,
                             bool shortest_path) {
  Real cosine = p.DotProduct(q);
  Quaternion t_q(0, 0, 0, 0);

  // Do we need to invert rotation?
  if (cosine < 0.0f && shortest_path) {
    cosine = -cosine;
    t_q = -q;
  } else {
    t_q = q;
  }

  if (abs(cosine) < 1 - 1e-03) {
    // Standard case (slerp)
    Real sine = sqrt(1 - cosine * cosine);
    Real angle = atan2(sine, cosine);
    Real inv_sine = 1.0f / sine;
    Real coeff0 = sin((1.0f - t) * angle) * inv_sine;
    Real coeff1 = sin(t * angle) * inv_sine;
    return coeff0 * p + coeff1 * t_q;
  } else {
    // There are two situations:
    // 1. "p" and "q" are very close (cosine ~= +1), so we can do a linear
    //    interpolation safely.
    // 2. "p" and "q" are almost inverse of each other (cosine ~= -1), there
    //    are an infinite number of possibilities interpolation. but we haven't
    //    have method to fix this case, so just use linear interpolation here.
    Quaternion tmp = (1.0f - t) * p + t * t_q;
    // taking the complement requires renormalisation
    tmp.Normalize();
    return tmp;
  }
}
//-----------------------------------------------------------------------

bool Quaternion::operator==(const Quaternion& q) const {
  return x == q.x && y == q.y && z == q.z && w == q.w;
}
//-----------------------------------------------------------------------

Vector3 Quaternion::operator*(Vector3 v) {
  Vector3 uv(0, 0, 0), uuv(0, 0, 0);
  Vector3 qvec(x, y, z);
  uv = qvec.CrossProduct(v);
  uuv = qvec.CrossProduct(uv);
  uv *= (2.0f * w);
  uuv *= 2.0f;
  return v + uv + uuv;
}
//-----------------------------------------------------------------------

Quaternion Quaternion::operator+(const Quaternion& q) const {
  return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}
//----------------------------------------------------------------------

Quaternion Quaternion::operator-(const Quaternion& q) const {
  return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
}
//-----------------------------------------------------------------------

Quaternion Quaternion::operator*(const Quaternion& q) const {
  // NOTE:  Multiplication is not generally commutative, so in most
  // cases p*q != q*p.

  return Quaternion(w * q.w - x * q.x - y * q.y - z * q.z,
                    w * q.x + x * q.w + y * q.z - z * q.y,
                    w * q.y + y * q.w + z * q.x - x * q.z,
                    w * q.z + z * q.w + x * q.y - y * q.x);
}
//-----------------------------------------------------------------------

Quaternion Quaternion::operator*(Real scalar) const {
  return Quaternion(scalar * w, scalar * x, scalar * y, scalar * z);
}
//-----------------------------------------------------------------------

Quaternion Quaternion::operator-() const { return Quaternion(-w, -x, -y, -z); }
//-----------------------------------------------------------------------

Quaternion operator*(Real scalar, const Quaternion& q) {
  return Quaternion(scalar * q.w, scalar * q.x, scalar * q.y, scalar * q.z);
}
//-----------------------------------------------------------------------

}  // namespace gnat
