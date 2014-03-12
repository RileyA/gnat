#include "gtest/gtest.h"
#include "util/color.h"

using gnat::ColorU8;

namespace {

TEST(ColorTest, Init) {
  ColorU8 c(0, 1, 2, 3);
  EXPECT_EQ(0, c.r);
  EXPECT_EQ(1, c.g);
  EXPECT_EQ(2, c.b);
  EXPECT_EQ(3, c.a);
}

TEST(ColorTest, SquareBrackets_Read) {
  ColorU8 c(0, 1, 2, 3);
  EXPECT_EQ(0, c[0]);
  EXPECT_EQ(1, c[1]);
  EXPECT_EQ(2, c[2]);
  EXPECT_EQ(3, c[3]);
}

TEST(ColorTest, SquareBrackets_Write) {
  ColorU8 c(0, 0, 0, 0);
  c[0] = 1;
  c[1] = 2;
  c[2] = 3;
  c[3] = 4;
  EXPECT_EQ(1, c[0]);
  EXPECT_EQ(2, c[1]);
  EXPECT_EQ(3, c[2]);
  EXPECT_EQ(4, c[3]);
}

}
