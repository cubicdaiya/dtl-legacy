
#include "../dtl.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>

typedef char elem;
typedef std::string sequence;

void strdiff3_test (sequence A, sequence B, sequence C, sequence S) {
  dtl::Diff3<elem, sequence> diff3(A, B, C);
  diff3.compose();
  if (!diff3.merge()) {
    fprintf(stderr, "conflict.\n");
    exit(-1);
  }
  if (S == diff3.getMergedSequence()) {
    std::cout << "successed : " << A << " " << B << " "  << C << " " << S << " " << diff3.getMergedSequence() << std::endl;
  } else { 
    std::cout << "failed    : " << A << " " << B << " "  << C << " " << S << " " << diff3.getMergedSequence() << std::endl;
  }
}

int main(int argc, char *argv[]){
  
  strdiff3_test("ab", "b", "bc", "abc");
  strdiff3_test("bc", "b", "ab", "abc");

  strdiff3_test("qqqabc", "abc", "abcdef", "qqqabcdef");
  strdiff3_test("abcdef", "abc", "qqqabc", "qqqabcdef");

  strdiff3_test("aiueo", "aeo", "aeKokaki", "aiueKokaki");
  strdiff3_test("aeKokaki", "aeo", "aiueo", "aiueKokaki");

  strdiff3_test("aaacccbbb", "aaabbb", "aaabbbqqq", "aaacccbbbqqq");
  strdiff3_test("aaabbbqqq", "aaabbb", "aaacccbbb", "aaacccbbbqqq");

  strdiff3_test("aeaacccbbb", "aaabbb", "aaabbbqqq",  "aeaacccbbbqqq");
  strdiff3_test("aaabbbqqq",  "aaabbb", "aeaacccbbb", "aeaacccbbbqqq");

  strdiff3_test("aeaacccbbb", "aaabbb", "aaabebbqqq",  "aeaacccbebbqqq");
  strdiff3_test("aaabebbqqq",  "aaabbb", "aeaacccbbb", "aeaacccbebbqqq");

  strdiff3_test("aaacccbbb",  "aaabbb", "aeaabbbqqq", "aeaacccbbbqqq");
  strdiff3_test("aeaabbbqqq", "aaabbb", "aaacccbbb",  "aeaacccbbbqqq");

  strdiff3_test("aaacccbbb",  "aaabbb", "aaabeebbeeqqq", "aaacccbeebbeeqqq");
  strdiff3_test("aaabeebbeeqqq", "aaabbb", "aaacccbbb",  "aaacccbeebbeeqqq");

  return 0;
}






