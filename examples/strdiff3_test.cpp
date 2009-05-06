
#include "../dtl.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef char elem;
typedef string sequence;

void merge_test(sequence A, sequence B, sequence C, sequence S);
void detect_conflict_test (sequence A, sequence B, sequence C);

void merge_test (sequence A, sequence B, sequence C, sequence S) {
  dtl::Diff3<elem, sequence> diff3(A, B, C);
  diff3.compose();
  if (!diff3.merge()) {
    fprintf(stderr, "conflict.\n");
    return;
  }
  if (S == diff3.getMergedSequence()) {
    cout << "successed : " << A << " " << B << " "  << C << " " << S << " " << diff3.getMergedSequence() << endl;
  } else { 
    cout << "failed    : " << A << " " << B << " "  << C << " " << S << " " << diff3.getMergedSequence() << endl;
  }
}

void detect_conflict_test (sequence A, sequence B, sequence C) {
  dtl::Diff3<elem, sequence> diff3(A, B, C);
  diff3.compose();
  if (!diff3.merge()) {
    cout << "detect conflict successed : " << A << " " << B << " "  << C << endl;
  } else {
    cout << "detect conflict failed    : " << A << " " << B << " "  << C << endl;
  }
}

int main(int, char**){
  
  cout << "merge test" << endl << endl;

  merge_test("ab", "b", "bc", "abc");
  merge_test("bc", "b", "ab", "abc");

  merge_test("qqqabc", "abc", "abcdef", "qqqabcdef");
  merge_test("abcdef", "abc", "qqqabc", "qqqabcdef");

  merge_test("aaacccbbb", "aaabbb", "aaabbbqqq", "aaacccbbbqqq");
  merge_test("aaabbbqqq", "aaabbb", "aaacccbbb", "aaacccbbbqqq");

  merge_test("aeaacccbbb", "aaabbb", "aaabbbqqq",  "aeaacccbbbqqq");
  merge_test("aaabbbqqq",  "aaabbb", "aeaacccbbb", "aeaacccbbbqqq");

  merge_test("aeaacccbbb", "aaabbb", "aaabebbqqq",  "aeaacccbebbqqq");
  merge_test("aaabebbqqq",  "aaabbb", "aeaacccbbb", "aeaacccbebbqqq");

  merge_test("aaacccbbb",  "aaabbb", "aeaabbbqqq", "aeaacccbbbqqq");
  merge_test("aeaabbbqqq", "aaabbb", "aaacccbbb",  "aeaacccbbbqqq");

  merge_test("aaacccbbb",  "aaabbb", "aaabeebbeeqqq", "aaacccbeebbeeqqq");
  merge_test("aaabeebbeeqqq", "aaabbb", "aaacccbbb",  "aaacccbeebbeeqqq");

  merge_test("aiueo", "aeo", "aeKokaki", "aiueKokaki");
  merge_test("aeKokaki", "aeo", "aiueo", "aiueKokaki");

  merge_test("1234567390", "1234567890", "1239567890", "1239567390");
  merge_test("1239567890", "1234567890", "1234567390", "1239567390");

  merge_test("qabcdef", "abcdef", "ab",      "qab");
  merge_test("ab",      "abcdef", "qabcdef", "qab");
  
  merge_test("abcdf", "abcdef", "acdef", "acdf");
  merge_test("acdef", "abcdef", "abcdf", "acdf");

  merge_test("acdef",   "abcdef", "abcdfaa",  "acdfaa");
  merge_test("abcdfaa", "abcdef", "acdef",   "acdfaa");

  cout << endl;

  cout << "detect conflict test" << endl << endl;
  detect_conflict_test("adc", "abc", "aec");
  detect_conflict_test("aec", "abc", "adc");

  
  return 0;
}






