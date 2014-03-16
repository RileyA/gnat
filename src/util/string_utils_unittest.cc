#include "gtest/gtest.h"
#include "util/string_utils.h"

using namespace gnat;

namespace {

TEST(StringUtilTest, IntToStr) {
  EXPECT_EQ("5", to_str((int)5));
}

}
