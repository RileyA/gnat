#include "gtest/gtest.h"
#include "util/file_utils.h"

int main(int argc, char** argv) {
  gnat::FileUtils::SetBaseFilePathFromArgv(argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
