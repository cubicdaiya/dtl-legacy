
#include "../dtl.hpp"
#include "common.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>

using namespace std;

int main(int argc, char *argv[]) {

  if (isFewArgs(argc)) {
    cerr << "few arguments" << endl;
    return -1;
  }

  string A(argv[1]);
  string B(argv[2]);
  typedef char elem;
  
  dtl::Diff<elem, string> d(A, B);
  d.compose();

  string s1(A);
  string s2 = d.patch(s1);
  d.composeUnifiedHunks();
  string s3 = d.uniPatch(s1);

  cout << "before:" << s1 << endl;
  cout << "after :" << s2 << endl;
  assert(B == s2);
  cout << "patch OK" << endl;
  
  cout << "before:" << s1 << endl;
  cout << "after :" << s3 << endl;
  assert(B == s3);
  cout << "unipatch OK" << endl;

  return 0;
}
