
#include "../dtl.hpp"
#include "common.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cassert>

int main(int argc, char *argv[]){
  
  if (isFewArgs(argc)) {
    perror("few arguments");
    return(EXIT_FAILURE);
  }

  std::string A(argv[1]);
  std::string B(argv[2]);
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
    return(EXIT_FAILURE);
  }

  typedef std::string elem;
  std::ifstream Aifs(A.c_str());
  std::ifstream Bifs(B.c_str());
  elem buf;
  std::vector<elem> ALines, BLines;
  std::ostringstream ossLine, ossInfo;

  while(getline(Aifs, buf)){
    ALines.push_back(buf);
  }
  while(getline(Bifs, buf)){
    BLines.push_back(buf);
  }
  
  dtl::Diff<elem, std::vector<elem> > d(ALines, BLines);
  d.compose();

  dtl::Ses<elem> ses = d.getSes();
  
  std::vector<elem> s1 = ALines;
  std::vector<elem> s2 = d.patch(s1, ses);

  // fpatch 
  assert(BLines == s2);
  std::cout << "fpatch OK" << std::endl;

  d.composeUnifiedHunks();
  std::vector<elem> s3 = d.uniPatch(s1);

  dtl::Diff<elem, std::vector<elem> > d2(s3, BLines);
  d2.compose();

  d2.composeUnifiedHunks();
  d2.printUnifiedFormat();

  // unipatch 
  assert(BLines == s3);
  std::cout << "unipatch OK" << std::endl;
  
  return 0;
}
