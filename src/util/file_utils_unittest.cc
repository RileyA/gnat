#include "gtest/gtest.h"
#include "util/file_utils.h"

namespace gnat {

TEST(FileUtilsTest, ReadFile) {
  String out;
  EXPECT_TRUE(FileUtils::ReadFile(FileUtils::GetBaseFilePath() +
                                  "data/test/file.txt", &out));
  EXPECT_EQ(String("Line 1\nLine 2\n"), out);
}

}  // namespace gnat
