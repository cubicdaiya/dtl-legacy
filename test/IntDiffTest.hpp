
#ifndef INTDIFF_TEST
#define INTDIFF_TEST

#include "dtl_test_common.hpp"

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

/**
 * IntDiffTest
 * check list is following
 * - editdistance
 * - LCS
 * - SES
 */
TEST_F (IntDiffTest, diff_test0) {
  EXPECT_EQ(0, cases[0].diff.getEditDistance());

  EXPECT_TRUE(cases[0].lcs_v.empty());

  ASSERT_TRUE(cases[0].ses_seq.empty());
}

TEST_F (IntDiffTest, diff_test1) {
  EXPECT_EQ(1,       cases[1].diff.getEditDistance());

  EXPECT_TRUE(cases[1].lcs_v.empty());

  ASSERT_EQ(1,       cases[1].ses_seq[0].first);
  ASSERT_EQ(SES_ADD, cases[1].ses_seq[0].second.type);
}

TEST_F (IntDiffTest, diff_test2) {
  EXPECT_EQ(1,          cases[2].diff.getEditDistance());

  EXPECT_TRUE(cases[2].lcs_v.empty());

  ASSERT_EQ(1,          cases[2].ses_seq[0].first);
  ASSERT_EQ(SES_DELETE, cases[2].ses_seq[0].second.type);
}

TEST_F (IntDiffTest, diff_test3) {
  EXPECT_EQ(8,          cases[3].diff.getEditDistance());

  EXPECT_EQ(3,          cases[3].lcs_v[0]);
  EXPECT_EQ(4,          cases[3].lcs_v[1]);
  EXPECT_EQ(5,          cases[3].lcs_v[2]);
  EXPECT_EQ(7,          cases[3].lcs_v[3]);
  EXPECT_EQ(9,          cases[3].lcs_v[4]);

  ASSERT_EQ(1,          cases[3].ses_seq[0].first);
  ASSERT_EQ(2,          cases[3].ses_seq[1].first);
  ASSERT_EQ(3,          cases[3].ses_seq[2].first);
  ASSERT_EQ(5,          cases[3].ses_seq[3].first);
  ASSERT_EQ(1,          cases[3].ses_seq[4].first);
  ASSERT_EQ(4,          cases[3].ses_seq[5].first);
  ASSERT_EQ(5,          cases[3].ses_seq[6].first);
  ASSERT_EQ(6,          cases[3].ses_seq[7].first);
  ASSERT_EQ(1,          cases[3].ses_seq[8].first);
  ASSERT_EQ(7,          cases[3].ses_seq[9].first);
  ASSERT_EQ(8,          cases[3].ses_seq[10].first);
  ASSERT_EQ(9,          cases[3].ses_seq[11].first);
  ASSERT_EQ(6,          cases[3].ses_seq[12].first);
  ASSERT_EQ(10,         cases[3].ses_seq[13].first);
  ASSERT_EQ(SES_DELETE, cases[3].ses_seq[0].second.type);
  ASSERT_EQ(SES_DELETE, cases[3].ses_seq[1].second.type);
  ASSERT_EQ(SES_COMMON, cases[3].ses_seq[2].second.type);
  ASSERT_EQ(SES_ADD,    cases[3].ses_seq[3].second.type);
  ASSERT_EQ(SES_ADD,    cases[3].ses_seq[4].second.type);
  ASSERT_EQ(SES_COMMON, cases[3].ses_seq[5].second.type);
  ASSERT_EQ(SES_COMMON, cases[3].ses_seq[6].second.type);
  ASSERT_EQ(SES_DELETE, cases[3].ses_seq[7].second.type);
  ASSERT_EQ(SES_ADD,    cases[3].ses_seq[8].second.type);
  ASSERT_EQ(SES_COMMON, cases[3].ses_seq[9].second.type);
  ASSERT_EQ(SES_DELETE, cases[3].ses_seq[10].second.type);
  ASSERT_EQ(SES_COMMON, cases[3].ses_seq[11].second.type);
  ASSERT_EQ(SES_ADD,    cases[3].ses_seq[12].second.type);
  ASSERT_EQ(SES_COMMON, cases[3].ses_seq[13].second.type);
}

TEST_F (IntDiffTest, diff_test4) {
  EXPECT_EQ(4,          cases[4].diff.getEditDistance());

  EXPECT_EQ(3,          cases[4].lcs_v[0]);
  EXPECT_EQ(4,          cases[4].lcs_v[1]);
  EXPECT_EQ(5,          cases[4].lcs_v[2]);
  
  ASSERT_EQ(1,          cases[4].ses_seq[0].first);
  ASSERT_EQ(2,          cases[4].ses_seq[1].first);
  ASSERT_EQ(3,          cases[4].ses_seq[2].first);
  ASSERT_EQ(5,          cases[4].ses_seq[3].first);
  ASSERT_EQ(1,          cases[4].ses_seq[4].first);
  ASSERT_EQ(4,          cases[4].ses_seq[5].first);
  ASSERT_EQ(5,          cases[4].ses_seq[6].first);
  ASSERT_EQ(SES_DELETE, cases[4].ses_seq[0].second.type);
  ASSERT_EQ(SES_DELETE, cases[4].ses_seq[1].second.type);
  ASSERT_EQ(SES_COMMON, cases[4].ses_seq[2].second.type);
  ASSERT_EQ(SES_ADD,    cases[4].ses_seq[3].second.type);
  ASSERT_EQ(SES_ADD,    cases[4].ses_seq[4].second.type);
  ASSERT_EQ(SES_COMMON, cases[4].ses_seq[5].second.type);
  ASSERT_EQ(SES_COMMON, cases[4].ses_seq[6].second.type);
}

#endif // INTDIFF_TEST
