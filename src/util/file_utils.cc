#include "gnat.h"

#include "util/file_utils.h"

#include <fstream>
#include <streambuf>

namespace gnat {

String BASE_FILE_PATH = "";

bool FileUtils::ReadFile(String filename, String* out) {
  std::ifstream file(filename.c_str());
  if (!file.good())
    return false;
  *out = String((std::istreambuf_iterator<char>(file)),
               std::istreambuf_iterator<char>());
  return true;
}

void FileUtils::SetBaseFilePath(String path) {
  BASE_FILE_PATH = path;
}

void FileUtils::SetBaseFilePathFromArgv(char** argv) {
  String tmp = argv[0];
  int backspaces = 0;
  while(backspaces < tmp.size() && tmp[tmp.size() - backspaces - 1] != '/')
    ++backspaces;
  BASE_FILE_PATH = tmp.substr(0, tmp.size() - backspaces);
}


}  // namespace gnat
