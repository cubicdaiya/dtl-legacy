
#include <iostream>
#include <vector>
#include "../src/dtl.hpp"

using namespace std;

using dtl::Diff;

int main(int, char**){
  typedef unsigned int uint;
  uint a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  uint b[10] = {3, 5, 1, 4, 5, 1, 7, 9, 6, 10};

  for (uint i=0;i<10;++i) {
    cout << a[i] << " ";
  }
  cout << endl;
  for (uint i=0;i<10;++i) {
    cout << b[i] << " ";
  }
  cout << endl;
  
  typedef uint elem;
  typedef vector<uint> sequence;
  sequence A(&a[0], &a[10]);
  sequence B(&b[0], &b[10]);
  Diff< elem, sequence > d(A, B);
  d.compose();
  // editDistance
  cout << "editDistance:" << d.getEditDistance() << endl;
  // Longest Common Subsequence
  sequence lcs_v = d.getLcsVec();
  cout << "LCS: ";
  for (sequence::iterator vit=lcs_v.begin();vit!=lcs_v.end();++vit) {
    cout << *vit << " ";
  }
  cout << endl;

  // Shortest Edit Script
  cout << "SES" << endl;
  d.printSES();

  return 0;
}
