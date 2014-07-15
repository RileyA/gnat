#ifndef gnat_gfx_math_geometry_aabb_H
#define gnat_gfx_math_geometry_aabb_H

#include "gnat.h"
#include "gfx/math/vector3.h"

namespace gnat {

// Axis-Aligned Bounding Box
class AABB {
 public:
  AABB(Vector3 min, Vector3 max);
  AABB();

  // Expand to fit |point|.
  Vector3 expand(Vector3 point);

  Vector3 min() { return min_; }
  Vector3 max() { return max_; }
  void set_min(Vector3 min) { min_ = min; }
  void set_max(Vector3 max) { max_ = max; }

 private:
  Vector3 min_;
  Vector3 max_;
};

}  // namespace gnat

#endif  // gnat_gfx_math_geometry_aabb_H
