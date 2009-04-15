
#include "../dtl.hpp"
#include "common.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>

int main(int, char**) {

  int a[10]      = {1, 2, 3, 4, 5, 6, 7, 3, 9, 10};
  int b[10]      = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int c[10]      = {1, 2, 3, 9, 5, 6, 7, 8, 9, 10};
  int answer[10] = {1, 2, 3, 9, 5, 6, 7, 3, 9, 10};

  std::cout << "a:";
  for (int i=0;i<10;++i) {
    std::cout << a[i] << " ";
  }
  std:: cout << std::endl;
  std::cout << "b:";
  for (int i=0;i<10;++i) {
    std::cout << b[i] << " ";
  }
  std:: cout << std::endl;
  std::cout << "c:";
  for (int i=0;i<10;++i) {
    std::cout << c[i] << " ";
  }
  std:: cout << std::endl;
  
  
  typedef int elem;
  typedef std::vector<int> sequence;
  sequence A(&a[0], &a[10]);
  sequence B(&b[0], &b[10]);
  sequence C(&c[0], &c[10]);
  sequence Answer(&answer[0], &answer[10]);
  dtl::Diff3<elem, sequence> diff3(A, B, C);
  diff3.compose();
  if (!diff3.merge()) {
    fprintf(stderr, "conflict.\n");
    exit(-1);
  }
  sequence s = diff3.getMergedSequence();
  sequence::iterator it;
  std::cout << "s:";
  for (it=s.begin();it!=s.end();++it) {
    std::cout << *it << " ";
  }
  std:: cout << std::endl;
  
  assert(s == Answer);
  std::cout << "intdiff3 OK" << std::endl;
  
  return 0;
}






