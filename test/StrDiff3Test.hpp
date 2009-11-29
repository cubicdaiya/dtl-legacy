
#ifndef STRDIFF3_TEST
#define STRDIFF3_TEST

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "../src/dtl.hpp"

using namespace std;
using namespace dtl;

class StrDiff3Test : public ::testing::Test
{
protected :
  typedef char elem;
  typedef string sequence;
  typedef struct case_t {
    sequence A;
    sequence B;
    sequence C;
    sequence S;
    Diff3< elem, sequence > diff3;
  } case_t;
  typedef vector< case_t > caseVec;

  caseVec cases;

  case_t createCase (sequence a, sequence b, sequence c, sequence s) {
    case_t ct;
    ct.A = a;
    ct.B = b;
    ct.C = c;
    ct.S = s;
    ct.diff3 = Diff3< elem, sequence >(a, b, c);
    ct.diff3.compose();
    return ct;
  }
  
  void SetUp() {
    cases.push_back(createCase("ab",            "b",          "bc",            "abc"));              // 0
    cases.push_back(createCase("bc",            "b",          "ab",            "abc"));              // 1
    cases.push_back(createCase("qqqabc",        "abc",        "abcdef",        "qqqabcdef"));        // 2
    cases.push_back(createCase("abcdef",        "abc",        "qqqabc",        "qqqabcdef"));        // 3
    cases.push_back(createCase("aaacccbbb",     "aaabbb",     "aaabbbqqq",     "aaacccbbbqqq"));     // 4
    cases.push_back(createCase("aaabbbqqq",     "aaabbb",     "aaacccbbb",     "aaacccbbbqqq"));     // 5
    cases.push_back(createCase("aeaacccbbb",    "aaabbb",     "aaabbbqqq",     "aeaacccbbbqqq"));    // 6
    cases.push_back(createCase("aaabbbqqq",     "aaabbb",     "aeaacccbbb",    "aeaacccbbbqqq"));    // 7
    cases.push_back(createCase("aeaacccbbb",    "aaabbb",     "aaabebbqqq",    "aeaacccbebbqqq"));   // 8
    cases.push_back(createCase("aaabebbqqq",    "aaabbb",     "aeaacccbbb",    "aeaacccbebbqqq"));   // 9
    cases.push_back(createCase("aaacccbbb",     "aaabbb",     "aeaabbbqqq",    "aeaacccbbbqqq"));    // 10
    cases.push_back(createCase("aeaabbbqqq",    "aaabbb",     "aaacccbbb",     "aeaacccbbbqqq"));    // 11
    cases.push_back(createCase("aaacccbbb",     "aaabbb",     "aaabeebbeeqqq", "aaacccbeebbeeqqq")); // 12
    cases.push_back(createCase("aaabeebbeeqqq", "aaabbb",     "aaacccbbb",     "aaacccbeebbeeqqq")); // 13
    cases.push_back(createCase("aiueo",         "aeo",        "aeKokaki",      "aiueKokaki"));       // 14
    cases.push_back(createCase("aeKokaki",      "aeo",        "aiueo",         "aiueKokaki"));       // 15
    cases.push_back(createCase("1234567390",    "1234567890", "1239567890",    "1239567390"));       // 16
    cases.push_back(createCase("1239567890",    "1234567890", "1234567390",    "1239567390"));       // 17
    cases.push_back(createCase("qabcdef",       "abcdef",     "ab",            "qab"));              // 18
    cases.push_back(createCase("ab",            "abcdef",     "qabcdef",       "qab"));              // 19
    cases.push_back(createCase("abcdf",         "abcdef",     "acdef",         "acdf"));             // 20
    cases.push_back(createCase("acdef",         "abcdef",     "abcdf",         "acdf"));             // 21
    cases.push_back(createCase("acdef",         "abcdef",     "abcdfaa",       "acdfaa"));           // 22
    cases.push_back(createCase("abcdfaa",       "abcdef",     "acdef",         "acdfaa"));           // 23
  }
  
  void TearDown () {}
  
};

#endif
