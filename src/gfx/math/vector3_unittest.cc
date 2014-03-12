#include "gfx/math/vector3.h"
#include "gtest/gtest.h"

using gnat::Vector3;

namespace {

TEST(Vector3Test, Add) {
  Vector3 v(1.0, 2.0, 3.0);
  EXPECT_EQ(v.x, 1.0);
  EXPECT_EQ(v.y, 2.0);
  EXPECT_EQ(v.z, 3.0);
}

}
