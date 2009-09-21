
#include "../src/dtl.hpp"
#include "common.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char *argv[]){

  if (isFewArgs(argc, 4)) {
    cerr << "few argument." << endl;
    return -1;
  }
  
  typedef char elem;
  typedef string sequence;
  sequence A(argv[1]);
  sequence B(argv[2]);
  sequence C(argv[3]);

  dtl::Diff3<elem, sequence> diff3(A, B, C);
  diff3.compose();
  //diff3.setConflictSeparators('<', '|', '=', '>');
  if (!diff3.merge()) {
    cerr << "conflict." << endl;
    return -1;
  }
  cout << "result:" << diff3.getMergedSequence() << endl;
  
  return 0;
}
