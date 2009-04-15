
#include <iostream>
#include <vector>
#include "../dtl.hpp"

int main(int, char**){
  
  int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int b[10] = {3, 5, 1, 4, 5, 1, 7, 9, 6, 10};

  for (int i=0;i<10;++i) {
    std::cout << a[i] << " ";
  }
  std:: cout << std::endl;
  for (int i=0;i<10;++i) {
    std::cout << b[i] << " ";
  }
  std:: cout << std::endl;
  std::vector<int> A(&a[0], &a[10]);
  std::vector<int> B(&b[0], &b[10]);
  dtl::Diff<int, std::vector<int> > d(A, B);
  d.compose();
  // editDistance
  std::cout << "editDistance:" << d.getEditDistance() << std::endl;
  // Longest Common Subsequence
  dtl::Lcs<int> lcs = d.getLcs();
  std::vector<int> lcs_v = lcs.getSequence();
  std::vector<int>::iterator vit;
  std::cout << "LCS: ";
  for (vit=lcs_v.begin();vit!=lcs_v.end();++vit) {
    std::cout << *vit << " ";
  }
  std::cout << std::endl;

  // Short Edit Script
  std::cout << "SES" << std::endl;
  dtl::Ses<int> ses = d.getSes();
  std::vector< std::pair<int, dtl::elemInfo> > ses_v = ses.getSequence();
  std::vector< std::pair<int, dtl::elemInfo> >::iterator it;

  it = ses_v.begin();
  for (it=ses_v.begin();it!=ses_v.end();++it) {
    switch (it->second.type) {
    case dtl::SES_ADD :
      std::cout << dtl::SES_MARK_ADD    << " " << it->first << std::endl;
      break;
    case dtl::SES_DELETE :
      std::cout << dtl::SES_MARK_DELETE << " " << it->first << std::endl;
      break;
    case dtl::SES_COMMON :
      std::cout << dtl::SES_MARK_COMMON << " " << it->first << std::endl;
      break;
    default :
      break;
    }
  }

  return 0;
}






