
#include <iostream>
#include <vector>
#include "../dtl.hpp"

using namespace std;

int main(int, char**){
  
  int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int b[10] = {3, 5, 1, 4, 5, 1, 7, 9, 6, 10};

  for (int i=0;i<10;++i) {
    cout << a[i] << " ";
  }
   cout << endl;
  for (int i=0;i<10;++i) {
    cout << b[i] << " ";
  }
   cout << endl;
  vector<int> A(&a[0], &a[10]);
  vector<int> B(&b[0], &b[10]);
  dtl::Diff<int, vector<int> > d(A, B);
  d.compose();
  // editDistance
  cout << "editDistance:" << d.getEditDistance() << endl;
  // Longest Common Subsequence
  dtl::Lcs<int> lcs = d.getLcs();
  vector<int> lcs_v = lcs.getSequence();
  vector<int>::iterator vit;
  cout << "LCS: ";
  for (vit=lcs_v.begin();vit!=lcs_v.end();++vit) {
    cout << *vit << " ";
  }
  cout << endl;

  // Short Edit Script
  cout << "SES" << endl;
  dtl::Ses<int> ses = d.getSes();
  vector< pair<int, dtl::elemInfo> > ses_v = ses.getSequence();
  vector< pair<int, dtl::elemInfo> >::iterator it;

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






