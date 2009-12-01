
#ifndef STRDIFF_TEST
#define STRDIFF_TEST

#include "dtl_test_common.hpp"

class StrDiffTest : public ::testing::Test
{
protected :
  typedef char elem;
  typedef string sequence;
  typedef pair< elem, elemInfo > sesElem;
  typedef vector< elem > elemVec;
  typedef vector< sesElem > sesElemVec;
  typedef vector < uniHunk< sesElem > > uniHunkVec;
  typedef struct case_t {
    sequence A;
    sequence B;
    Diff< elem, sequence > diff;
    elemVec lcs_v;
    sequence lcs_s;
    sesElemVec ses_seq;
    uniHunkVec hunk_v;
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
    elemVec lcs_v = c.diff.getLcsVec();
    c.lcs_s       = sequence(lcs_v.begin(), lcs_v.end());
    c.ses_seq     = c.diff.getSes().getSequence();
    c.hunk_v      = c.diff.getUniHunks();
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

/**
 * StrDiffTest
 * check list is following
 * - editdistance
 * - LCS
 * - SES
 * - Unified Format Difference(UDF)
 */

TEST_F (StrDiffTest, diff_test0) {
  
  EXPECT_EQ(2,          cases[0].diff.getEditDistance());
  
  EXPECT_EQ("ab",       cases[0].lcs_s);

  ASSERT_EQ('a',        cases[0].ses_seq[0].first);
  ASSERT_EQ('b',        cases[0].ses_seq[1].first);
  ASSERT_EQ('c',        cases[0].ses_seq[2].first);
  ASSERT_EQ('d',        cases[0].ses_seq[3].first);
  ASSERT_EQ(SES_COMMON, cases[0].ses_seq[0].second.type);
  ASSERT_EQ(SES_COMMON, cases[0].ses_seq[1].second.type);
  ASSERT_EQ(SES_DELETE, cases[0].ses_seq[2].second.type);
  ASSERT_EQ(SES_ADD,    cases[0].ses_seq[3].second.type);
  
  ASSERT_EQ(1,          cases[0].hunk_v[0].a);
  ASSERT_EQ(3,          cases[0].hunk_v[0].b);
  ASSERT_EQ(1,          cases[0].hunk_v[0].c);
  ASSERT_EQ(3,          cases[0].hunk_v[0].d);
  ASSERT_EQ('a',        cases[0].hunk_v[0].common[0][0].first);
  ASSERT_EQ('b',        cases[0].hunk_v[0].common[0][1].first);
  ASSERT_EQ('c',        cases[0].hunk_v[0].change[0].first);
  ASSERT_EQ('d',        cases[0].hunk_v[0].change[1].first);
  ASSERT_EQ(SES_COMMON, cases[0].hunk_v[0].common[0][0].second.type);
  ASSERT_EQ(SES_COMMON, cases[0].hunk_v[0].common[0][1].second.type);
  ASSERT_EQ(SES_DELETE, cases[0].hunk_v[0].change[0].second.type);
  ASSERT_EQ(SES_ADD,    cases[0].hunk_v[0].change[1].second.type);
  ASSERT_TRUE(cases[0].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test1) {
  EXPECT_EQ(6,          cases[1].diff.getEditDistance());

  EXPECT_EQ("acbdabed", cases[1].lcs_s);
  
  ASSERT_EQ('a',        cases[1].ses_seq[0].first);
  ASSERT_EQ('c',        cases[1].ses_seq[1].first);
  ASSERT_EQ('e',        cases[1].ses_seq[2].first);
  ASSERT_EQ('b',        cases[1].ses_seq[3].first);
  ASSERT_EQ('d',        cases[1].ses_seq[4].first);
  ASSERT_EQ('e',        cases[1].ses_seq[5].first);
  ASSERT_EQ('a',        cases[1].ses_seq[6].first);
  ASSERT_EQ('c',        cases[1].ses_seq[7].first);
  ASSERT_EQ('b',        cases[1].ses_seq[8].first);
  ASSERT_EQ('b',        cases[1].ses_seq[9].first);
  ASSERT_EQ('a',        cases[1].ses_seq[10].first);
  ASSERT_EQ('b',        cases[1].ses_seq[11].first);
  ASSERT_EQ('e',        cases[1].ses_seq[12].first);
  ASSERT_EQ('d',        cases[1].ses_seq[13].first);
  ASSERT_EQ(SES_COMMON, cases[1].ses_seq[0].second.type);
  ASSERT_EQ(SES_COMMON, cases[1].ses_seq[1].second.type);
  ASSERT_EQ(SES_ADD,    cases[1].ses_seq[2].second.type);
  ASSERT_EQ(SES_COMMON, cases[1].ses_seq[3].second.type);
  ASSERT_EQ(SES_COMMON, cases[1].ses_seq[4].second.type);
  ASSERT_EQ(SES_DELETE, cases[1].ses_seq[5].second.type);
  ASSERT_EQ(SES_COMMON, cases[1].ses_seq[6].second.type);
  ASSERT_EQ(SES_DELETE, cases[1].ses_seq[7].second.type);
  ASSERT_EQ(SES_COMMON, cases[1].ses_seq[8].second.type);
  ASSERT_EQ(SES_ADD,    cases[1].ses_seq[9].second.type);
  ASSERT_EQ(SES_ADD,    cases[1].ses_seq[10].second.type);
  ASSERT_EQ(SES_ADD,    cases[1].ses_seq[11].second.type);
  ASSERT_EQ(SES_COMMON, cases[1].ses_seq[12].second.type);
  ASSERT_EQ(SES_COMMON, cases[1].ses_seq[13].second.type);

  ASSERT_EQ(1,          cases[1].hunk_v[0].a);
  ASSERT_EQ(10,         cases[1].hunk_v[0].b);
  ASSERT_EQ(1,          cases[1].hunk_v[0].c);
  ASSERT_EQ(12,         cases[1].hunk_v[0].d);
  ASSERT_EQ('a',        cases[1].hunk_v[0].common[0][0].first);
  ASSERT_EQ('c',        cases[1].hunk_v[0].common[0][1].first);
  ASSERT_EQ('e',        cases[1].hunk_v[0].change[0].first);
  ASSERT_EQ('b',        cases[1].hunk_v[0].change[1].first);
  ASSERT_EQ('d',        cases[1].hunk_v[0].change[2].first);
  ASSERT_EQ('e',        cases[1].hunk_v[0].change[3].first);
  ASSERT_EQ('a',        cases[1].hunk_v[0].change[4].first);
  ASSERT_EQ('c',        cases[1].hunk_v[0].change[5].first);
  ASSERT_EQ('b',        cases[1].hunk_v[0].change[6].first);
  ASSERT_EQ('b',        cases[1].hunk_v[0].change[7].first);
  ASSERT_EQ('a',        cases[1].hunk_v[0].change[8].first);
  ASSERT_EQ('b',        cases[1].hunk_v[0].change[9].first);
  ASSERT_EQ('e',        cases[1].hunk_v[0].change[10].first);
  ASSERT_EQ('d',        cases[1].hunk_v[0].change[11].first);
  ASSERT_EQ(SES_COMMON, cases[1].hunk_v[0].common[0][0].second.type);
  ASSERT_EQ(SES_COMMON, cases[1].hunk_v[0].common[0][1].second.type);
  ASSERT_EQ(SES_ADD,    cases[1].hunk_v[0].change[0].second.type);
  ASSERT_EQ(SES_COMMON, cases[1].hunk_v[0].change[1].second.type);
  ASSERT_EQ(SES_COMMON, cases[1].hunk_v[0].change[2].second.type);
  ASSERT_EQ(SES_DELETE, cases[1].hunk_v[0].change[3].second.type);
  ASSERT_EQ(SES_COMMON, cases[1].hunk_v[0].change[4].second.type);
  ASSERT_EQ(SES_DELETE, cases[1].hunk_v[0].change[5].second.type);
  ASSERT_EQ(SES_COMMON, cases[1].hunk_v[0].change[6].second.type);
  ASSERT_EQ(SES_ADD,    cases[1].hunk_v[0].change[7].second.type);
  ASSERT_EQ(SES_ADD,    cases[1].hunk_v[0].change[8].second.type);
  ASSERT_EQ(SES_ADD,    cases[1].hunk_v[0].change[9].second.type);
  ASSERT_EQ(SES_COMMON, cases[1].hunk_v[0].change[10].second.type);
  ASSERT_EQ(SES_COMMON, cases[1].hunk_v[0].change[11].second.type);
  ASSERT_TRUE(cases[1].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test2) {
  EXPECT_EQ(6,          cases[2].diff.getEditDistance());

  EXPECT_EQ("acf",      cases[2].lcs_s);

  ASSERT_EQ('d',        cases[2].ses_seq[0].first);
  ASSERT_EQ('a',        cases[2].ses_seq[1].first);
  ASSERT_EQ('b',        cases[2].ses_seq[2].first);
  ASSERT_EQ('c',        cases[2].ses_seq[3].first);
  ASSERT_EQ('d',        cases[2].ses_seq[4].first);
  ASSERT_EQ('e',        cases[2].ses_seq[5].first);
  ASSERT_EQ('f',        cases[2].ses_seq[6].first);
  ASSERT_EQ('e',        cases[2].ses_seq[7].first);
  ASSERT_EQ('a',        cases[2].ses_seq[8].first);
  ASSERT_EQ(SES_ADD,    cases[2].ses_seq[0].second.type);
  ASSERT_EQ(SES_COMMON, cases[2].ses_seq[1].second.type);
  ASSERT_EQ(SES_DELETE, cases[2].ses_seq[2].second.type);
  ASSERT_EQ(SES_COMMON, cases[2].ses_seq[3].second.type);
  ASSERT_EQ(SES_DELETE, cases[2].ses_seq[4].second.type);
  ASSERT_EQ(SES_DELETE, cases[2].ses_seq[5].second.type);
  ASSERT_EQ(SES_COMMON, cases[2].ses_seq[6].second.type);
  ASSERT_EQ(SES_ADD,    cases[2].ses_seq[7].second.type);
  ASSERT_EQ(SES_ADD,    cases[2].ses_seq[8].second.type);

  ASSERT_EQ(1,          cases[2].hunk_v[0].a);
  ASSERT_EQ(6,          cases[2].hunk_v[0].b);
  ASSERT_EQ(1,          cases[2].hunk_v[0].c);
  ASSERT_EQ(6,          cases[2].hunk_v[0].d);
  ASSERT_EQ('d',        cases[2].hunk_v[0].change[0].first);
  ASSERT_EQ('a',        cases[2].hunk_v[0].change[1].first);
  ASSERT_EQ('b',        cases[2].hunk_v[0].change[2].first);
  ASSERT_EQ('c',        cases[2].hunk_v[0].change[3].first);
  ASSERT_EQ('d',        cases[2].hunk_v[0].change[4].first);
  ASSERT_EQ('e',        cases[2].hunk_v[0].change[5].first);
  ASSERT_EQ('f',        cases[2].hunk_v[0].change[6].first);
  ASSERT_EQ('e',        cases[2].hunk_v[0].change[7].first);
  ASSERT_EQ('a',        cases[2].hunk_v[0].change[8].first);
  ASSERT_TRUE(cases[2].hunk_v[0].common[0].empty());
  ASSERT_EQ(SES_ADD,    cases[2].hunk_v[0].change[0].second.type);
  ASSERT_EQ(SES_COMMON, cases[2].hunk_v[0].change[1].second.type);
  ASSERT_EQ(SES_DELETE, cases[2].hunk_v[0].change[2].second.type);
  ASSERT_EQ(SES_COMMON, cases[2].hunk_v[0].change[3].second.type);
  ASSERT_EQ(SES_DELETE, cases[2].hunk_v[0].change[4].second.type);
  ASSERT_EQ(SES_DELETE, cases[2].hunk_v[0].change[5].second.type);
  ASSERT_EQ(SES_COMMON, cases[2].hunk_v[0].change[6].second.type);
  ASSERT_EQ(SES_ADD,    cases[2].hunk_v[0].change[7].second.type);
  ASSERT_EQ(SES_ADD,    cases[2].hunk_v[0].change[8].second.type);
  ASSERT_TRUE(cases[2].hunk_v[0].common[1].empty());
  
}

TEST_F (StrDiffTest, diff_test3) {
  EXPECT_EQ(4,          cases[3].diff.getEditDistance());

  EXPECT_EQ("bcba",     cases[3].lcs_s);

  ASSERT_EQ('a',        cases[3].ses_seq[0].first);
  ASSERT_EQ('b',        cases[3].ses_seq[1].first);
  ASSERT_EQ('d',        cases[3].ses_seq[2].first);
  ASSERT_EQ('c',        cases[3].ses_seq[3].first);
  ASSERT_EQ('a',        cases[3].ses_seq[4].first);
  ASSERT_EQ('b',        cases[3].ses_seq[5].first);
  ASSERT_EQ('d',        cases[3].ses_seq[6].first);
  ASSERT_EQ('a',        cases[3].ses_seq[7].first);
  ASSERT_EQ(SES_DELETE, cases[3].ses_seq[0].second.type);
  ASSERT_EQ(SES_COMMON, cases[3].ses_seq[1].second.type);
  ASSERT_EQ(SES_ADD,    cases[3].ses_seq[2].second.type);
  ASSERT_EQ(SES_COMMON, cases[3].ses_seq[3].second.type);
  ASSERT_EQ(SES_ADD,    cases[3].ses_seq[4].second.type);
  ASSERT_EQ(SES_COMMON, cases[3].ses_seq[5].second.type);
  ASSERT_EQ(SES_DELETE, cases[3].ses_seq[6].second.type);
  ASSERT_EQ(SES_COMMON, cases[3].ses_seq[7].second.type);

  ASSERT_EQ(1,          cases[3].hunk_v[0].a);
  ASSERT_EQ(6,          cases[3].hunk_v[0].b);
  ASSERT_EQ(1,          cases[3].hunk_v[0].c);
  ASSERT_EQ(6,          cases[3].hunk_v[0].d);
  ASSERT_TRUE(cases[3].hunk_v[0].common[0].empty());
  ASSERT_EQ('a',        cases[3].hunk_v[0].change[0].first);
  ASSERT_EQ('b',        cases[3].hunk_v[0].change[1].first);
  ASSERT_EQ('d',        cases[3].hunk_v[0].change[2].first);
  ASSERT_EQ('c',        cases[3].hunk_v[0].change[3].first);
  ASSERT_EQ('a',        cases[3].hunk_v[0].change[4].first);
  ASSERT_EQ('b',        cases[3].hunk_v[0].change[5].first);
  ASSERT_EQ('d',        cases[3].hunk_v[0].change[6].first);
  ASSERT_EQ('a',        cases[3].hunk_v[0].change[7].first);
  ASSERT_EQ(SES_DELETE, cases[3].hunk_v[0].change[0].second.type);
  ASSERT_EQ(SES_COMMON, cases[3].hunk_v[0].change[1].second.type);
  ASSERT_EQ(SES_ADD,    cases[3].hunk_v[0].change[2].second.type);
  ASSERT_EQ(SES_COMMON, cases[3].hunk_v[0].change[3].second.type);
  ASSERT_EQ(SES_ADD,    cases[3].hunk_v[0].change[4].second.type);
  ASSERT_EQ(SES_COMMON, cases[3].hunk_v[0].change[5].second.type);
  ASSERT_EQ(SES_DELETE, cases[3].hunk_v[0].change[6].second.type);
  ASSERT_EQ(SES_COMMON, cases[3].hunk_v[0].change[7].second.type);
  ASSERT_TRUE(cases[3].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test4) {
  EXPECT_EQ(1,          cases[4].diff.getEditDistance());

  EXPECT_EQ("bokko",    cases[4].lcs_s);

  ASSERT_EQ('b',        cases[4].ses_seq[0].first);
  ASSERT_EQ('o',        cases[4].ses_seq[1].first);
  ASSERT_EQ('k',        cases[4].ses_seq[2].first);
  ASSERT_EQ('k',        cases[4].ses_seq[3].first);
  ASSERT_EQ('k',        cases[4].ses_seq[4].first);
  ASSERT_EQ('o',        cases[4].ses_seq[5].first);
  ASSERT_EQ(SES_COMMON, cases[4].ses_seq[0].second.type);
  ASSERT_EQ(SES_COMMON, cases[4].ses_seq[1].second.type);
  ASSERT_EQ(SES_COMMON, cases[4].ses_seq[2].second.type);
  ASSERT_EQ(SES_COMMON, cases[4].ses_seq[3].second.type);
  ASSERT_EQ(SES_ADD,    cases[4].ses_seq[4].second.type);
  ASSERT_EQ(SES_COMMON, cases[4].ses_seq[5].second.type);

  ASSERT_EQ(2,          cases[4].hunk_v[0].a);
  ASSERT_EQ(4,          cases[4].hunk_v[0].b);
  ASSERT_EQ(2,          cases[4].hunk_v[0].c);
  ASSERT_EQ(5,          cases[4].hunk_v[0].d);
  ASSERT_EQ('o',        cases[4].hunk_v[0].common[0][0].first);
  ASSERT_EQ('k',        cases[4].hunk_v[0].common[0][1].first);
  ASSERT_EQ('k',        cases[4].hunk_v[0].common[0][2].first);
  ASSERT_EQ('k',        cases[4].hunk_v[0].change[0].first);
  ASSERT_EQ('o',        cases[4].hunk_v[0].change[1].first);
  ASSERT_EQ(SES_COMMON, cases[4].hunk_v[0].common[0][0].second.type);
  ASSERT_EQ(SES_COMMON, cases[4].hunk_v[0].common[0][1].second.type);
  ASSERT_EQ(SES_COMMON, cases[4].hunk_v[0].common[0][2].second.type);
  ASSERT_EQ(SES_ADD,    cases[4].hunk_v[0].change[0].second.type);
  ASSERT_EQ(SES_COMMON, cases[4].hunk_v[0].change[1].second.type);
  ASSERT_TRUE(cases[4].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test5) {
  EXPECT_EQ(0,  cases[5].diff.getEditDistance());

  EXPECT_EQ("", cases[5].lcs_s);

  ASSERT_TRUE(cases[5].ses_seq.empty());

  ASSERT_TRUE(cases[5].hunk_v.empty());
}

TEST_F (StrDiffTest, diff_test6) {
  EXPECT_EQ(1,          cases[6].diff.getEditDistance());

  EXPECT_EQ("",         cases[6].lcs_s);

  ASSERT_EQ('a',        cases[6].ses_seq[0].first);
  ASSERT_EQ(SES_DELETE, cases[6].ses_seq[0].second.type);

  ASSERT_EQ(1,          cases[6].hunk_v[0].a);
  ASSERT_EQ(1,          cases[6].hunk_v[0].b);
  ASSERT_EQ(1,          cases[6].hunk_v[0].c);
  ASSERT_EQ(0,          cases[6].hunk_v[0].d);
  ASSERT_TRUE(cases[6].hunk_v[0].common[0].empty());
  ASSERT_EQ('a',        cases[6].hunk_v[0].change[0].first);
  ASSERT_EQ(SES_DELETE, cases[6].hunk_v[0].change[0].second.type);
  ASSERT_TRUE(cases[6].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test7) {
  EXPECT_EQ(1,       cases[7].diff.getEditDistance());

  EXPECT_EQ("",      cases[7].lcs_s);

  ASSERT_EQ('b',     cases[7].ses_seq[0].first);
  ASSERT_EQ(SES_ADD, cases[7].ses_seq[0].second.type);

  ASSERT_EQ(1,       cases[7].hunk_v[0].a);
  ASSERT_EQ(0,       cases[7].hunk_v[0].b);
  ASSERT_EQ(1,       cases[7].hunk_v[0].c);
  ASSERT_EQ(1,       cases[7].hunk_v[0].d);
  ASSERT_TRUE(cases[7].hunk_v[0].common[0].empty());
  ASSERT_EQ('b',     cases[7].hunk_v[0].change[0].first);
  ASSERT_EQ(SES_ADD, cases[7].hunk_v[0].change[0].second.type);
  ASSERT_TRUE(cases[7].hunk_v[0].common[1].empty());
}

#endif
