#include "gfx/math/geometry/aabb.h"

#include <algorithm>

namespace gnat {

AABB::AABB(Vector3 min, Vector3 max) 
  : min_(min),
    max_(max) {}
//---------------------------------------------------------------------------

AABB::AABB()
  : min_(Vector3::ZERO),
    max_(Vector3::ZERO) {}
//---------------------------------------------------------------------------

// Expand to fit |point|.
void AABB::expand(Vector3 point) {
  using std::min;
  using std::max;
  min_ = Vector3(min(min_.x, point.x),
                 min(min_.y, point.y),
                 min(min_.z, point.z));
  max_ = Vector3(max(max_.x, point.x),
                 max(max_.y, point.y),
                 max(max_.z, point.z));
}
//---------------------------------------------------------------------------

}  // namespace gnat
