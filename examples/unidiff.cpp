
#include "../dtl.hpp"
#include "common.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SEPARATE_SIZE (3)
#define CONTEXT_SIZE (3)

void showStats (std::string fp1, std::string fp2) 
{
  const int MAX_LENGTH = 255;
  time_t rawtime[2];
  struct tm *timeinfo[2];
  char time_format[] = "%Y-%m-%d %H:%M:%S %z";
  struct stat st[2];

  if (stat(fp1.c_str(), &st[0]) == -1) {
    perror("argv1");
    exit(-1);
  }
  if (stat(fp2.c_str(), &st[1]) == -1) {
    perror("argv2");
    exit(-1);
  }

  char buf[2][MAX_LENGTH + 1];
  rawtime[0] = st[0].st_mtime;
  timeinfo[0] = localtime(&rawtime[0]);
  strftime(buf[0], MAX_LENGTH, time_format, timeinfo[0]);
  std::cout << "--- " << fp1 << '\t' << buf[0] << std::endl;
  rawtime[1] = st[1].st_mtime;
  timeinfo[1] = localtime(&rawtime[1]);
  strftime(buf[1], MAX_LENGTH, time_format, timeinfo[1]);
  std::cout << "+++ " << fp2 << '\t' << buf[1] << std::endl;
}

void unifiedDiff (std::string fp1, std::string fp2) 
{
  typedef std::string elem;
  std::ifstream Aifs(fp1.c_str());
  std::ifstream Bifs(fp2.c_str());
  elem buf;
  std::vector<elem> ALines, BLines;
  std::ostringstream ossLine, ossInfo;

  while(getline(Aifs, buf)){
    ALines.push_back(buf);
  }
  while(getline(Bifs, buf)){
    BLines.push_back(buf);
  }
  
  dtl::Diff<elem, std::vector<elem> > diff(ALines, BLines);
  diff.onHuge();
  //diff.onUnserious();
  diff.compose();

  // type unihunk definition test
  dtl::uniHunk< std::pair<elem, dtl::elemInfo> > hunk;

  if (diff.getEditDistance() > 0) {
    showStats(fp1, fp2);             // show file info
  }

  diff.composeUnifiedHunks();
  diff.printUnifiedFormat();
}


int main(int argc, char *argv[])
{
  if (isFewArgs(argc)) {
    perror("few argument.");
    return(EXIT_FAILURE);
  }
  
  std::string s1(argv[1]);
  std::string s2(argv[2]);
  bool isFileExist = true;

  if (!fileExists(s1)) {
    perror(s1.c_str());
    isFileExist = false;
  }

  if (!fileExists(s2)) {
    perror(s2.c_str());
    isFileExist = false;
  }
  
  if (!isFileExist) {
    return(EXIT_FAILURE);
  }

  unifiedDiff(s1, s2);
  return 0;
}
