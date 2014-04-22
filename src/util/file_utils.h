#ifndef gnat_util_file_utils_H
#define gnat_util_file_utils_H

#include "gnat.h"

namespace gnat {

extern String BASE_FILE_PATH;

class FileUtils {
 public:
  static bool ReadFile(String filename, String* out);
  static void SetBaseFilePath(String path);
  static void SetBaseFilePathFromArgv(char** argv);
  static const String& GetBaseFilePath() { return BASE_FILE_PATH; }
};

}  // namespace gnat

#endif  // gnat_util_file_utils_H
