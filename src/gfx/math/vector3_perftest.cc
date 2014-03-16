#include "gfx/math/vector3.h"
#include "util/timer.h"
#include "gtest/gtest.h"

using gnat::Vector3;
using gnat::Timer;

namespace {

const static int kTestIterations = 1000000;

// TODO experiment with SSE2 stuffs!

TEST(Vector3PerfTest, Add) {
  Timer t;
  t.Start();
  Vector3 v1(10.0, 20.0, 30.0);
  Vector3 v2(30.0, 20.0, 10.0);
  for (int i = 0; i < kTestIterations; ++i) {
    EXPECT_EQ(v1 + v2, Vector3(40.0, 40.0, 40.0));
  }
  t.Stop();
  printf("Time: %f\n", t.GetTimeMilliseconds());
}

}
