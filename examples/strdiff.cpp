
#include "../dtl.hpp"
#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>

int main(int argc, char *argv[]){

  if (argc < 3) {
    perror("few argument.");
    return(EXIT_FAILURE);
  }
  
  std::string A(argv[1]);
  std::string B(argv[2]);
  typedef char elem;

  dtl::Diff<elem, std::string> d(A, B);
  d.compose();
  
  // editDistance
  std::cout << "editDistance:" << d.getEditDistance() << std::endl;

  // Long Common Subsequence
  dtl::Lcs<elem> lcs = d.getLcs();
  std::vector<elem> lcs_v = lcs.getSequence();
  std::vector<elem>::iterator vit;
  std::string lcs_s(lcs_v.begin(), lcs_v.end());
  std::cout << "LCS:" << lcs_s << std::endl;

  // Short Edit Script
  std::cout << "SES" << std::endl;
  dtl::Ses<elem> ses = d.getSes();
  std::vector< std::pair<elem, dtl::elemInfo> > ses_v = ses.getSequence();
  std::vector< std::pair<elem, dtl::elemInfo> >::iterator it;

  it = ses_v.begin();
  for (it=ses_v.begin();it!=ses_v.end();++it) {
    switch (it->second.type) {
    case dtl::SES_ADD :
      std::cout << "A " << it->first << std::endl;
      break;
    case dtl::SES_DELETE :
      std::cout << "D " << it->first << std::endl;
      break;
    case dtl::SES_COMMON :
      std::cout << "C " << it->first << std::endl;
      break;
    default :
      break;
    }
  }

  return 0;
}






