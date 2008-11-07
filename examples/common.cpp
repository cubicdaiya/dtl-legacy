
#include "common.hpp"

bool fileExists (std::string& fs) {
  FILE *fp;
  bool ret = true;
  if ((fp = fopen(fs.c_str(), "r")) == NULL) {
    ret = false;
  }
  fclose(fp);
  return ret;
}

bool isFewArgs (int argc, int limit) {
  bool ret = false;
  if (argc < limit) {
    ret = true;
  }
  return ret;
}
