
#include "../dtl.hpp"
#include "common.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>

int main(int argc, char *argv[]){

  if (isFewArgs(argc, 4)) {
    perror("few argument.");
    return(EXIT_FAILURE);
  }
  
  std::string A(argv[1]);
  std::string B(argv[2]);
  std::string C(argv[3]);
  typedef char elem;
  typedef std::string sequence;

  dtl::Diff3<elem, sequence> diff3(A, B, C);
  diff3.compose();
  

  if (!diff3.merge()) {
    perror("conflict.");
    return(EXIT_FAILURE);
  }
  
  return 0;
}






