#include "gnat.h"

#include "vector4.h"

namespace gnat {

const Vector4 Vector4::ZERO(0.0, 0.0, 0.0, 1.0);
const Vector4 Vector4::UNIT_X(1.0, 0.0, 0.0, 1.0);
const Vector4 Vector4::UNIT_Y(0.0, 1.0, 0.0, 1.0);
const Vector4 Vector4::UNIT_Z(0.0, 0.0, 1.0, 1.0);
const Vector4 Vector4::NEGATIVE_UNIT_X( -1.0,  0.0,  0.0, 1.0);
const Vector4 Vector4::NEGATIVE_UNIT_Y(  0.0, -1.0,  0.0, 1.0);
const Vector4 Vector4::NEGATIVE_UNIT_Z(  0.0,  0.0, -1.0, 1.0);
const Vector4 Vector4::UNIT_SCALE(1.0, 1.0, 1.0, 1.0);

#ifdef USE_VECTOR4_SIMD
Vector4 Vector4::operator +(const Vector4& v) const {
  Vector4 out = v;
  vector4_add_simd(&x, &out.x);
  return out;
}
#endif

}
