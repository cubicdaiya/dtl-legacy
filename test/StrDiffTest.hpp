
#ifndef STRDIFFTEST
#define STRDIFFTEST

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <utility>
#include "../src/dtl.hpp"

using namespace std;
using namespace dtl;

class StrDiffTest : public ::testing::Test
{
protected :
  typedef char elem;
  typedef string sequence;
  typedef pair< elem, elemInfo > sesElem;
  typedef vector< elem > elemVec;
  typedef vector< sesElem > sesElemVec;
  typedef struct case_t {
    sequence A;
    sequence B;
    Diff< elem, sequence > diff;
    elemVec lcs_v;
    sequence lcs_s;
    sesElemVec ses_seq;
  } case_t;
  typedef vector< case_t > caseVec;

  caseVec cases;

  case_t createCase (sequence a, sequence b) {
    case_t c;
    c.A = a;
    c.B = b;
    c.diff = Diff< elem, sequence >(a, b);
    c.diff.compose();
    elemVec lcs_v = c.diff.getLcsVec();
    c.lcs_s = sequence(lcs_v.begin(), lcs_v.end());
    c.ses_seq = c.diff.getSes().getSequence();    
    return c;
  }

  void SetUp() {
    cases.push_back(createCase("abc",        "abd"));          // 0
    cases.push_back(createCase("acbdeacbed", "acebdabbabed")); // 1
    cases.push_back(createCase("abcdef",     "dacfea"));       // 2
    cases.push_back(createCase("abcbda",     "bdcaba"));       // 3
    cases.push_back(createCase("bokko",      "bokkko"));       // 4
    cases.push_back(createCase("",           ""));             // 5
    cases.push_back(createCase("a",          ""));             // 6
    cases.push_back(createCase("",           "b"));            // 7
  }

  void TearDown () {}
  
};

#endif
