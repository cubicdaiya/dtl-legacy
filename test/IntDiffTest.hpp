
#ifndef INTDIFFTEST
#define INTDIFFTEST

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <utility>
#include "../src/dtl.hpp"

using namespace std;
using namespace dtl;

class IntDiffTest : public ::testing::Test
{
protected :
  typedef int elem;
  typedef vector< int > sequence;
  typedef sequence elemVec;
  typedef pair< elem, elemInfo > sesElem;
  typedef vector< sesElem > sesElemVec;
  typedef struct case_t {
    sequence A;
    sequence B;
    Diff< elem, sequence > diff;
    elemVec lcs_v;
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
    c.lcs_v = c.diff.getLcsVec();
    c.ses_seq = c.diff.getSes().getSequence();    
    return c;
  }

  void SetUp() {
    cases.push_back(createCase(sequence(0), sequence(0))); // 0
    sequence B1;
    B1.push_back(1);
    cases.push_back(createCase(sequence(0), B1));          // 1
    sequence A2;
    A2.push_back(1);
    cases.push_back(createCase(A2, sequence(0)));          // 2
    int a4[]   = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int b4[]   = {3, 5, 1, 4, 5, 1, 7, 9, 6, 10};
    int a4siz  = sizeof(a4) / sizeof(int);
    int b4siz  = sizeof(b4) / sizeof(int);
    sequence A4(&a4[0], &a4[a4siz]);
    sequence B4(&b4[0], &b4[b4siz]);
    cases.push_back(createCase(A4, B4));                   // 3
    int a5[]   = {1, 2, 3, 4, 5};
    int b5[]   = {3, 5, 1, 4, 5};
    int a5siz  = sizeof(a5) / sizeof(int);
    int b5siz  = sizeof(b5) / sizeof(int);
    sequence A5(&a5[0], &a5[a5siz]);
    sequence B5(&b5[0], &b5[b5siz]);
    cases.push_back(createCase(A5, B5));                   // 4
  }

  void TearDown () {}
  
};

#endif
