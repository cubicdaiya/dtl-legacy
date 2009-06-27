
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
  
  string A(argv[1]);
  string B(argv[2]);
  typedef char elem;
  typedef pair<elem, dtl::elemInfo> sesElem;

  dtl::Diff<elem, string> d(A, B);
  //d.onOnlyEditDistance();
  d.compose();
  
  // editDistance
  cout << "editDistance:" << d.getEditDistance() << endl;

  // Longest Common Subsequence
  dtl::Lcs<elem> lcs = d.getLcs();
  vector<elem> lcs_v = lcs.getSequence();
  string lcs_s(lcs_v.begin(), lcs_v.end());
  cout << "LCS:" << lcs_s << endl;

  // Shortest Edit Script
  cout << "SES" << endl;
  dtl::Ses<elem> ses = d.getSes();
  vector< sesElem > ses_v = ses.getSequence();

  for_each(ses_v.begin(), ses_v.end(), dtl::PrintChange< sesElem >());

  return 0;
}
