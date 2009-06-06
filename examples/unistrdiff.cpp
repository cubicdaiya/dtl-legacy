
#include "../dtl.hpp"
#include "common.hpp"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char *argv[]){

  if (isFewArgs(argc)) {
    cerr << "few argument" << endl;
    return -1;
  }
  
  typedef char elem;
  string A(argv[1]);
  string B(argv[2]);

  dtl::Diff<elem, string > d(A, B);
  d.compose();
  d.composeUnifiedHunks();

  // editDistance
  cout << "editDistance:" << d.getEditDistance() << endl;

  // Longest Common Subsequence
  dtl::Lcs<elem> lcs = d.getLcs();
  vector<elem> lcs_v = lcs.getSequence();
  string lcs_s(lcs_v.begin(), lcs_v.end());
  cout << "LCS:" << lcs_s << endl;

  // print Unified Format
  d.printUnifiedFormat();

  return 0;
}






