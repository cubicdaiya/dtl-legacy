
#include "../dtl.hpp"
#include "common.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cassert>

using namespace std;

int main(int argc, char *argv[]){
  
  if (isFewArgs(argc)) {
    cerr << "few arguments" << endl;
    return -1;
  }

  string A(argv[1]);
  string B(argv[2]);
  bool isFileExist = true;

  if (!fileExists(A)) {
    perror(A.c_str());
    isFileExist = false;
  }

  if (!fileExists(B)) {
    perror(B.c_str());
    isFileExist = false;
  }

  if (!isFileExist) {
    return -1;
  }

  typedef string elem;
  ifstream Aifs(A.c_str());
  ifstream Bifs(B.c_str());
  elem buf;
  vector<elem> ALines, BLines;
  ostringstream ossLine, ossInfo;

  while(getline(Aifs, buf)){
    ALines.push_back(buf);
  }
  while(getline(Bifs, buf)){
    BLines.push_back(buf);
  }
  
  dtl::Diff<elem, vector<elem> > d(ALines, BLines);
  d.compose();

  dtl::Ses<elem> ses = d.getSes();
  
  vector<elem> s1 = ALines;
  vector<elem> s2 = d.patch(s1, ses);

  // fpatch 
  assert(BLines == s2);
  cout << "fpatch OK" << endl;

  d.composeUnifiedHunks();
  vector<elem> s3 = d.uniPatch(s1);

  dtl::Diff<elem, vector<elem> > d2(s3, BLines);
  d2.compose();

  d2.composeUnifiedHunks();

  // unipatch 
  assert(BLines == s3);
  cout << "unipatch OK" << endl;
  
  return 0;
}
