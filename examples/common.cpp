
#include "common.hpp"

bool fileExists (std::string fs) {
  FILE *fp;
  bool ret = true;
  if ((fp = fopen(fs.c_str(), "r")) == NULL) {
    ret = false;
  }
  return ret;
}

