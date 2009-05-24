
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
  bool fileExist = true;

  if (!isFileExist(A)) {
    perror(A.c_str());
    cerr << "file A does not exist" << endl;
    fileExist = false;
  }

  if (!isFileExist(B)) {
    cerr << "file B does not exist" << endl;
    perror(B.c_str());
    fileExist = false;
  }

  if (!fileExist) {
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
  vector<elem> s2 = d.patch(s1);

  // fpatch 
  assert(BLines == s2);
  cout << "fpatch successed" << endl;

  d.composeUnifiedHunks();
  vector<elem> s3 = d.uniPatch(s1);

  // unipatch 
  assert(BLines == s3);
  cout << "unipatch successed" << endl;
  
  return 0;
}
