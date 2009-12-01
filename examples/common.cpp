
#include "common.hpp"

bool isFileExist (string& fs) {
  struct stat s;
  if (stat(fs.c_str(), &s) == -1) {
    return false;
  }
  return true;
}

bool isFewArgs (int argc, int limit) {
  bool ret = false;
  if (argc < limit) {
    ret = true;
  }
  return ret;
}

