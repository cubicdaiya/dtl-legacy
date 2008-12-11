
#include "../dtl.hpp"
#include "common.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>

int main(int argc, char *argv[]) {

  if (isFewArgs(argc)) {
    perror("few arguments");
    return(EXIT_FAILURE);
  }

  std::string A(argv[1]);
  std::string B(argv[2]);
  typedef char elem;
  
  dtl::Diff<elem, std::string> d(A, B);
  d.compose();

  dtl::Ses<elem> ses = d.getSes();
  
  std::string s1(A);
  std::string s2 = d.patch(s1, ses);
  d.composeUnifiedHunks();
  std::string s3 = d.uniPatch(s1);

  std::cout << "before:" << s1 << std::endl;
  std::cout << "after :" << s2 << std::endl;
  assert(B == s2);
  std::cout << "patch OK" << std::endl;
  
  std::cout << "before:" << s1 << std::endl;
  std::cout << "after :" << s3 << std::endl;
  assert(B == s3);
  std::cout << "unipatch OK" << std::endl;

  return 0;
}
