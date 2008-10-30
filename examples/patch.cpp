
#include "../dtl.hpp"
#include <iostream>
#include <vector>
#include <cassert>

int main(int argc, char *argv[]){
  std::string A(argv[1]);
  std::string B(argv[2]);
  typedef char elem;
  
  dtl::Diff<elem, std::string> d(A, B);
  d.compose();

  dtl::Ses<elem> ses = d.getSes();
  
  std::string s1(A);
  std::string s2 = d.patch(s1, ses);

  std::cout << "before:" << s1 << std::endl;
  std::cout << "after:" << s2 << std::endl;
  assert(B == s2);
  std::cout << "OK" << std::endl;

  return 0;
}
