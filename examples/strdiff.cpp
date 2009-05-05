
#include "../dtl.hpp"
#include "common.hpp"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char *argv[]){

  if (isFewArgs(argc)) {
    cerr << "few argument" << endl;
    return(EXIT_FAILURE);
  }
  
  string A(argv[1]);
  string B(argv[2]);
  typedef char elem;

  dtl::Diff<elem, string> d(A, B);
  //d.onOnlyEditDistance();
  d.compose();
  
  // editDistance
  cout << "editDistance:" << d.getEditDistance() << endl;

  // Longest Common Subsequence
  dtl::Lcs<elem> lcs = d.getLcs();
  vector<elem> lcs_v = lcs.getSequence();
  vector<elem>::iterator vit;
  string lcs_s(lcs_v.begin(), lcs_v.end());
  cout << "LCS:" << lcs_s << endl;

  // Short Edit Script
  cout << "SES" << endl;
  dtl::Ses<elem> ses = d.getSes();
  vector< pair<elem, dtl::elemInfo> > ses_v = ses.getSequence();
  vector< pair<elem, dtl::elemInfo> >::iterator it;

  it = ses_v.begin();
  for (it=ses_v.begin();it!=ses_v.end();++it) {
    switch (it->second.type) {
    case dtl::SES_ADD :
      cout << dtl::SES_MARK_ADD    << " " << it->first << endl;
      break;
    case dtl::SES_DELETE :
      cout << dtl::SES_MARK_DELETE << " " << it->first << endl;
      break;
    case dtl::SES_COMMON :
      cout << dtl::SES_MARK_COMMON << " " << it->first << endl;
      break;
    default :
      break;
    }
  }

  return 0;
}






