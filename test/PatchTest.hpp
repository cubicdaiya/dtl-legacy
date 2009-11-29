
#ifndef PATCH_TEST
#define PATCH_TEST

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "../src/dtl.hpp"

using namespace std;
using namespace dtl;

class PatchTest : public ::testing::Test
{
protected :
  typedef char elem;
  typedef string sequence;
  typedef struct case_t {
    sequence A;
    sequence B;
    Diff< elem, sequence > diff;
  } case_t;
  typedef vector< case_t > caseVec;

  caseVec cases;

  case_t createCase (sequence a, sequence b) {
    case_t c;
    c.A = a;
    c.B = b;
    c.diff = Diff< elem, sequence >(a, b);
    c.diff.compose();
    c.diff.composeUnifiedHunks();
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
