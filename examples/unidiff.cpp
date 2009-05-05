
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

using namespace std;

void showStats (string fp1, string fp2) 
{
  const int MAX_LENGTH = 255;
  time_t rawtime[2];
  struct tm *timeinfo[2];
  char time_format[] = "%Y-%m-%d %H:%M:%S %z";
  struct stat st[2];
  
  if (stat(fp1.c_str(), &st[0]) == -1) {
    cerr << "argv1 is invalid." << endl;
    exit(-1);
  }
  if (stat(fp2.c_str(), &st[1]) == -1) {
    cerr << "argv2 is invalid" << endl;
    exit(-1);
  }

  char buf[2][MAX_LENGTH + 1];
  rawtime[0] = st[0].st_mtime;
  timeinfo[0] = localtime(&rawtime[0]);
  strftime(buf[0], MAX_LENGTH, time_format, timeinfo[0]);
  cout << "--- " << fp1 << '\t' << buf[0] << endl;
  rawtime[1] = st[1].st_mtime;
  timeinfo[1] = localtime(&rawtime[1]);
  strftime(buf[1], MAX_LENGTH, time_format, timeinfo[1]);
  cout << "+++ " << fp2 << '\t' << buf[1] << endl;
}

void unifiedDiff (string fp1, string fp2) 
{
  typedef string elem;
  ifstream Aifs(fp1.c_str());
  ifstream Bifs(fp2.c_str());
  elem buf;
  vector<elem> ALines, BLines;
  ostringstream ossLine, ossInfo;

  while(getline(Aifs, buf)){
    ALines.push_back(buf);
  }
  while(getline(Bifs, buf)){
    BLines.push_back(buf);
  }
  
  dtl::Diff<elem, vector<elem> > diff(ALines, BLines);
  diff.onHuge();
  //diff.onUnserious();
  diff.compose();

  // type unihunk definition test
  dtl::uniHunk< pair<elem, dtl::elemInfo> > hunk;

  if (diff.getEditDistance() > 0) {
    showStats(fp1, fp2);             // show file info
  }

  diff.composeUnifiedHunks();
  diff.printUnifiedFormat();
}


int main(int argc, char *argv[])
{
  if (isFewArgs(argc)) {
    cerr << "few argument." << endl;
    return -1;
  }
  
  string s1(argv[1]);
  string s2(argv[2]);
  bool isFileExist = true;

  if (!fileExists(s1)) {
    cerr << s1 << " is invalid." << endl;
    perror(s1.c_str());
    isFileExist = false;
  }

  if (!fileExists(s2)) {
    cerr << s2 << " is invalid." << endl;
    isFileExist = false;
  }
  
  if (!isFileExist) {
    return -1;
  }

  unifiedDiff(s1, s2);
  return 0;
}
