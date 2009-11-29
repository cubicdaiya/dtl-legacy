
#include <gtest/gtest.h>
#include "StrDiffTest.hpp"
#include "IntDiffTest.hpp"
#include "StrDiff3Test.hpp"
#include "PatchTest.hpp"

/**
 * StrDiffTest
 */
TEST_F (StrDiffTest, test0) {
  
  EXPECT_EQ(cases[0].diff.getEditDistance(), 2);
  
  EXPECT_EQ(cases[0].lcs_s, "ab");

  ASSERT_EQ(cases[0].ses_seq[0].first, 'a');
  ASSERT_EQ(cases[0].ses_seq[1].first, 'b');
  ASSERT_EQ(cases[0].ses_seq[2].first, 'c');
  ASSERT_EQ(cases[0].ses_seq[3].first, 'd');
  ASSERT_EQ(cases[0].ses_seq[0].second.type, SES_COMMON);
  ASSERT_EQ(cases[0].ses_seq[1].second.type, SES_COMMON);
  ASSERT_EQ(cases[0].ses_seq[2].second.type, SES_DELETE);
  ASSERT_EQ(cases[0].ses_seq[3].second.type, SES_ADD);
}

TEST_F (StrDiffTest, test1) {
  EXPECT_EQ(cases[1].diff.getEditDistance(), 6);

  EXPECT_EQ(cases[1].lcs_s, "acbdabed");
  
  ASSERT_EQ(cases[1].ses_seq[0].first,  'a');
  ASSERT_EQ(cases[1].ses_seq[1].first,  'c');
  ASSERT_EQ(cases[1].ses_seq[2].first,  'e');
  ASSERT_EQ(cases[1].ses_seq[3].first,  'b');
  ASSERT_EQ(cases[1].ses_seq[4].first,  'd');
  ASSERT_EQ(cases[1].ses_seq[5].first,  'e');
  ASSERT_EQ(cases[1].ses_seq[6].first,  'a');
  ASSERT_EQ(cases[1].ses_seq[7].first,  'c');
  ASSERT_EQ(cases[1].ses_seq[8].first,  'b');
  ASSERT_EQ(cases[1].ses_seq[9].first,  'b');
  ASSERT_EQ(cases[1].ses_seq[10].first, 'a');
  ASSERT_EQ(cases[1].ses_seq[11].first, 'b');
  ASSERT_EQ(cases[1].ses_seq[12].first, 'e');
  ASSERT_EQ(cases[1].ses_seq[13].first, 'd');
  ASSERT_EQ(cases[1].ses_seq[0].second.type,  SES_COMMON);
  ASSERT_EQ(cases[1].ses_seq[1].second.type,  SES_COMMON);
  ASSERT_EQ(cases[1].ses_seq[2].second.type,  SES_ADD);
  ASSERT_EQ(cases[1].ses_seq[3].second.type,  SES_COMMON);
  ASSERT_EQ(cases[1].ses_seq[4].second.type,  SES_COMMON);
  ASSERT_EQ(cases[1].ses_seq[5].second.type,  SES_DELETE);
  ASSERT_EQ(cases[1].ses_seq[6].second.type,  SES_COMMON);
  ASSERT_EQ(cases[1].ses_seq[7].second.type,  SES_DELETE);
  ASSERT_EQ(cases[1].ses_seq[8].second.type,  SES_COMMON);
  ASSERT_EQ(cases[1].ses_seq[9].second.type,  SES_ADD);
  ASSERT_EQ(cases[1].ses_seq[10].second.type, SES_ADD);
  ASSERT_EQ(cases[1].ses_seq[11].second.type, SES_ADD);
  ASSERT_EQ(cases[1].ses_seq[12].second.type, SES_COMMON);
  ASSERT_EQ(cases[1].ses_seq[13].second.type, SES_COMMON);
}

TEST_F (StrDiffTest, test2) {
  EXPECT_EQ(cases[2].diff.getEditDistance(), 6);

  EXPECT_EQ(cases[2].lcs_s, "acf");

  ASSERT_EQ(cases[2].ses_seq[0].first, 'd');
  ASSERT_EQ(cases[2].ses_seq[1].first, 'a');
  ASSERT_EQ(cases[2].ses_seq[2].first, 'b');
  ASSERT_EQ(cases[2].ses_seq[3].first, 'c');
  ASSERT_EQ(cases[2].ses_seq[4].first, 'd');
  ASSERT_EQ(cases[2].ses_seq[5].first, 'e');
  ASSERT_EQ(cases[2].ses_seq[6].first, 'f');
  ASSERT_EQ(cases[2].ses_seq[7].first, 'e');
  ASSERT_EQ(cases[2].ses_seq[8].first, 'a');
  ASSERT_EQ(cases[2].ses_seq[0].second.type,  SES_ADD);
  ASSERT_EQ(cases[2].ses_seq[1].second.type,  SES_COMMON);
  ASSERT_EQ(cases[2].ses_seq[2].second.type,  SES_DELETE);
  ASSERT_EQ(cases[2].ses_seq[3].second.type,  SES_COMMON);
  ASSERT_EQ(cases[2].ses_seq[4].second.type,  SES_DELETE);
  ASSERT_EQ(cases[2].ses_seq[5].second.type,  SES_DELETE);
  ASSERT_EQ(cases[2].ses_seq[6].second.type,  SES_COMMON);
  ASSERT_EQ(cases[2].ses_seq[7].second.type,  SES_ADD);
  ASSERT_EQ(cases[2].ses_seq[8].second.type,  SES_ADD);
}

TEST_F (StrDiffTest, test3) {
  EXPECT_EQ(cases[3].diff.getEditDistance(), 4);

  EXPECT_EQ(cases[3].lcs_s, "bcba");

  ASSERT_EQ(cases[3].ses_seq[0].first, 'a');
  ASSERT_EQ(cases[3].ses_seq[1].first, 'b');
  ASSERT_EQ(cases[3].ses_seq[2].first, 'd');
  ASSERT_EQ(cases[3].ses_seq[3].first, 'c');
  ASSERT_EQ(cases[3].ses_seq[4].first, 'a');
  ASSERT_EQ(cases[3].ses_seq[5].first, 'b');
  ASSERT_EQ(cases[3].ses_seq[6].first, 'd');
  ASSERT_EQ(cases[3].ses_seq[7].first, 'a');
  ASSERT_EQ(cases[3].ses_seq[0].second.type, SES_DELETE);
  ASSERT_EQ(cases[3].ses_seq[1].second.type, SES_COMMON);
  ASSERT_EQ(cases[3].ses_seq[2].second.type, SES_ADD);
  ASSERT_EQ(cases[3].ses_seq[3].second.type, SES_COMMON);
  ASSERT_EQ(cases[3].ses_seq[4].second.type, SES_ADD);
  ASSERT_EQ(cases[3].ses_seq[5].second.type, SES_COMMON);
  ASSERT_EQ(cases[3].ses_seq[6].second.type, SES_DELETE);
  ASSERT_EQ(cases[3].ses_seq[7].second.type, SES_COMMON);
}

TEST_F (StrDiffTest, test4) {
  EXPECT_EQ(cases[4].diff.getEditDistance(), 1);

  EXPECT_EQ(cases[4].lcs_s, "bokko");

  ASSERT_EQ(cases[4].ses_seq[0].first, 'b');
  ASSERT_EQ(cases[4].ses_seq[1].first, 'o');
  ASSERT_EQ(cases[4].ses_seq[2].first, 'k');
  ASSERT_EQ(cases[4].ses_seq[3].first, 'k');
  ASSERT_EQ(cases[4].ses_seq[4].first, 'k');
  ASSERT_EQ(cases[4].ses_seq[5].first, 'o');
  ASSERT_EQ(cases[4].ses_seq[0].second.type, SES_COMMON);
  ASSERT_EQ(cases[4].ses_seq[1].second.type, SES_COMMON);
  ASSERT_EQ(cases[4].ses_seq[2].second.type, SES_COMMON);
  ASSERT_EQ(cases[4].ses_seq[3].second.type, SES_COMMON);
  ASSERT_EQ(cases[4].ses_seq[4].second.type, SES_ADD);
  ASSERT_EQ(cases[4].ses_seq[5].second.type, SES_COMMON);
}

TEST_F (StrDiffTest, test5) {
  EXPECT_EQ(cases[5].diff.getEditDistance(), 0);

  EXPECT_EQ(cases[5].lcs_s, "");

  ASSERT_TRUE(cases[5].ses_seq.empty());
}

TEST_F (StrDiffTest, test6) {
  EXPECT_EQ(cases[6].diff.getEditDistance(), 1);

  EXPECT_EQ(cases[6].lcs_s, "");

  ASSERT_EQ(cases[6].ses_seq[0].first, 'a');
  ASSERT_EQ(cases[6].ses_seq[0].second.type, SES_DELETE);
}

TEST_F (StrDiffTest, test7) {
  EXPECT_EQ(cases[7].diff.getEditDistance(), 1);

  EXPECT_EQ(cases[7].lcs_s, "");

  ASSERT_EQ(cases[7].ses_seq[0].first, 'b');
  ASSERT_EQ(cases[7].ses_seq[0].second.type, SES_ADD);
}

/**
 * IntDiffTest
 */
TEST_F (IntDiffTest, test0) {
  EXPECT_EQ(cases[0].diff.getEditDistance(), 0);

  EXPECT_TRUE(cases[0].lcs_v.empty());

  ASSERT_TRUE(cases[0].ses_seq.empty());

}

TEST_F (IntDiffTest, test1) {
  EXPECT_EQ(cases[1].diff.getEditDistance(), 1);

  EXPECT_TRUE(cases[1].lcs_v.empty());

  ASSERT_EQ(cases[1].ses_seq[0].first, 1);
  ASSERT_EQ(cases[1].ses_seq[0].second.type, SES_ADD);
}

TEST_F (IntDiffTest, test2) {
  EXPECT_EQ(cases[2].diff.getEditDistance(), 1);

  EXPECT_TRUE(cases[2].lcs_v.empty());

  ASSERT_EQ(cases[2].ses_seq[0].first, 1);
  ASSERT_EQ(cases[2].ses_seq[0].second.type, SES_DELETE);
}

TEST_F (IntDiffTest, test3) {
  EXPECT_EQ(cases[3].diff.getEditDistance(), 8);

  EXPECT_EQ(cases[3].lcs_v[0], 3);
  EXPECT_EQ(cases[3].lcs_v[1], 4);
  EXPECT_EQ(cases[3].lcs_v[2], 5);
  EXPECT_EQ(cases[3].lcs_v[3], 7);
  EXPECT_EQ(cases[3].lcs_v[4], 9);

  ASSERT_EQ(cases[3].ses_seq[0].first,  1);
  ASSERT_EQ(cases[3].ses_seq[1].first,  2);
  ASSERT_EQ(cases[3].ses_seq[2].first,  3);
  ASSERT_EQ(cases[3].ses_seq[3].first,  5);
  ASSERT_EQ(cases[3].ses_seq[4].first,  1);
  ASSERT_EQ(cases[3].ses_seq[5].first,  4);
  ASSERT_EQ(cases[3].ses_seq[6].first,  5);
  ASSERT_EQ(cases[3].ses_seq[7].first,  6);
  ASSERT_EQ(cases[3].ses_seq[8].first,  1);
  ASSERT_EQ(cases[3].ses_seq[9].first,  7);
  ASSERT_EQ(cases[3].ses_seq[10].first, 8);
  ASSERT_EQ(cases[3].ses_seq[11].first, 9);
  ASSERT_EQ(cases[3].ses_seq[12].first, 6);
  ASSERT_EQ(cases[3].ses_seq[13].first, 10);
  ASSERT_EQ(cases[3].ses_seq[0].second.type,  SES_DELETE);
  ASSERT_EQ(cases[3].ses_seq[1].second.type,  SES_DELETE);
  ASSERT_EQ(cases[3].ses_seq[2].second.type,  SES_COMMON);
  ASSERT_EQ(cases[3].ses_seq[3].second.type,  SES_ADD);
  ASSERT_EQ(cases[3].ses_seq[4].second.type,  SES_ADD);
  ASSERT_EQ(cases[3].ses_seq[5].second.type,  SES_COMMON);
  ASSERT_EQ(cases[3].ses_seq[6].second.type,  SES_COMMON);
  ASSERT_EQ(cases[3].ses_seq[7].second.type,  SES_DELETE);
  ASSERT_EQ(cases[3].ses_seq[8].second.type,  SES_ADD);
  ASSERT_EQ(cases[3].ses_seq[9].second.type,  SES_COMMON);
  ASSERT_EQ(cases[3].ses_seq[10].second.type, SES_DELETE);
  ASSERT_EQ(cases[3].ses_seq[11].second.type, SES_COMMON);
  ASSERT_EQ(cases[3].ses_seq[12].second.type, SES_ADD);
  ASSERT_EQ(cases[3].ses_seq[13].second.type, SES_COMMON);
}

TEST_F (IntDiffTest, test4) {
  EXPECT_EQ(cases[4].diff.getEditDistance(), 4);

  EXPECT_EQ(cases[4].lcs_v[0], 3);
  EXPECT_EQ(cases[4].lcs_v[1], 4);
  EXPECT_EQ(cases[4].lcs_v[2], 5);
  
  ASSERT_EQ(cases[4].ses_seq[0].first, 1);
  ASSERT_EQ(cases[4].ses_seq[1].first, 2);
  ASSERT_EQ(cases[4].ses_seq[2].first, 3);
  ASSERT_EQ(cases[4].ses_seq[3].first, 5);
  ASSERT_EQ(cases[4].ses_seq[4].first, 1);
  ASSERT_EQ(cases[4].ses_seq[5].first, 4);
  ASSERT_EQ(cases[4].ses_seq[6].first, 5);
  ASSERT_EQ(cases[4].ses_seq[0].second.type, SES_DELETE);
  ASSERT_EQ(cases[4].ses_seq[1].second.type, SES_DELETE);
  ASSERT_EQ(cases[4].ses_seq[2].second.type, SES_COMMON);
  ASSERT_EQ(cases[4].ses_seq[3].second.type, SES_ADD);
  ASSERT_EQ(cases[4].ses_seq[4].second.type, SES_ADD);
  ASSERT_EQ(cases[4].ses_seq[5].second.type, SES_COMMON);
  ASSERT_EQ(cases[4].ses_seq[6].second.type, SES_COMMON);
}

/**
 * StrDiff3Test
 */
TEST_F (StrDiff3Test, test0) {
  ASSERT_TRUE(cases[0].diff3.merge());
  ASSERT_EQ(cases[0].diff3.getMergedSequence(),  cases[0].S);
}

TEST_F (StrDiff3Test, test1) {
  ASSERT_TRUE(cases[1].diff3.merge());
  ASSERT_EQ(cases[1].diff3.getMergedSequence(),  cases[1].S);
}

TEST_F (StrDiff3Test, test2) {
  ASSERT_TRUE(cases[2].diff3.merge());
  ASSERT_EQ(cases[2].diff3.getMergedSequence(),  cases[2].S);
}

TEST_F (StrDiff3Test, test3) {
  ASSERT_TRUE(cases[3].diff3.merge());
  ASSERT_EQ(cases[3].diff3.getMergedSequence(),  cases[3].S);
}

TEST_F (StrDiff3Test, test4) {
  ASSERT_TRUE(cases[4].diff3.merge());
  ASSERT_EQ(cases[4].diff3.getMergedSequence(),  cases[4].S);
}

TEST_F (StrDiff3Test, test5) {
  ASSERT_TRUE(cases[5].diff3.merge());
  ASSERT_EQ(cases[5].diff3.getMergedSequence(),  cases[5].S);
}

TEST_F (StrDiff3Test, test6) {
  ASSERT_TRUE(cases[6].diff3.merge());
  ASSERT_EQ(cases[6].diff3.getMergedSequence(),  cases[6].S);
}

TEST_F (StrDiff3Test, test7) {
  ASSERT_TRUE(cases[7].diff3.merge());
  ASSERT_EQ(cases[7].diff3.getMergedSequence(),  cases[7].S);
}

TEST_F (StrDiff3Test, test8) {
  ASSERT_TRUE(cases[8].diff3.merge());
  ASSERT_EQ(cases[8].diff3.getMergedSequence(),  cases[8].S);
}

TEST_F (StrDiff3Test, test9) {
  ASSERT_TRUE(cases[9].diff3.merge());
  ASSERT_EQ(cases[9].diff3.getMergedSequence(),  cases[9].S);
}

TEST_F (StrDiff3Test, test10) {
  ASSERT_TRUE(cases[10].diff3.merge());
  ASSERT_EQ(cases[10].diff3.getMergedSequence(), cases[10].S);
}

TEST_F (StrDiff3Test, test11) {
  ASSERT_TRUE(cases[11].diff3.merge());
  ASSERT_EQ(cases[11].diff3.getMergedSequence(),  cases[11].S);
}

TEST_F (StrDiff3Test, test12) {
  ASSERT_TRUE(cases[12].diff3.merge());
  ASSERT_EQ(cases[12].diff3.getMergedSequence(),  cases[12].S);
}

TEST_F (StrDiff3Test, test13) {
  ASSERT_TRUE(cases[13].diff3.merge());
  ASSERT_EQ(cases[13].diff3.getMergedSequence(),  cases[13].S);
}

TEST_F (StrDiff3Test, test14) {
  ASSERT_TRUE(cases[14].diff3.merge());
  ASSERT_EQ(cases[14].diff3.getMergedSequence(),  cases[14].S);
}

TEST_F (StrDiff3Test, test15) {
  ASSERT_TRUE(cases[15].diff3.merge());
  ASSERT_EQ(cases[15].diff3.getMergedSequence(),  cases[15].S);
}

TEST_F (StrDiff3Test, test16) {
  ASSERT_TRUE(cases[16].diff3.merge());
  ASSERT_EQ(cases[16].diff3.getMergedSequence(),  cases[16].S);
}

TEST_F (StrDiff3Test, test17) {
  ASSERT_TRUE(cases[17].diff3.merge());
  ASSERT_EQ(cases[17].diff3.getMergedSequence(),  cases[17].S);
}

TEST_F (StrDiff3Test, test18) {
  ASSERT_TRUE(cases[18].diff3.merge());
  ASSERT_EQ(cases[18].diff3.getMergedSequence(),  cases[18].S);
}

TEST_F (StrDiff3Test, test19) {
  ASSERT_TRUE(cases[19].diff3.merge());
  ASSERT_EQ(cases[19].diff3.getMergedSequence(),  cases[19].S);
}

TEST_F (StrDiff3Test, test20) {
  ASSERT_TRUE(cases[20].diff3.merge());
  ASSERT_EQ(cases[20].diff3.getMergedSequence(),  cases[20].S);
}

TEST_F (StrDiff3Test, test21) {
  ASSERT_TRUE(cases[21].diff3.merge());
  ASSERT_EQ(cases[21].diff3.getMergedSequence(),  cases[21].S);
}

TEST_F (StrDiff3Test, test22) {
  ASSERT_TRUE(cases[22].diff3.merge());
  ASSERT_EQ(cases[22].diff3.getMergedSequence(),  cases[22].S);
}

TEST_F (StrDiff3Test, test23) {
  ASSERT_TRUE(cases[23].diff3.merge());
  ASSERT_EQ(cases[23].diff3.getMergedSequence(),  cases[23].S);
}

/**
 * PatchTest
 */
TEST_F (PatchTest, test0) {
  ASSERT_EQ(cases[0].diff.patch(cases[0].A),    cases[0].B);
  ASSERT_EQ(cases[0].diff.uniPatch(cases[0].A), cases[0].B);
}

TEST_F (PatchTest, test1) {
  ASSERT_EQ(cases[1].diff.patch(cases[1].A),    cases[1].B);
  ASSERT_EQ(cases[1].diff.uniPatch(cases[1].A), cases[1].B);
}

TEST_F (PatchTest, test2) {
  ASSERT_EQ(cases[2].diff.patch(cases[2].A),    cases[2].B);
  ASSERT_EQ(cases[2].diff.uniPatch(cases[2].A), cases[2].B);
}

TEST_F (PatchTest, test3) {
  ASSERT_EQ(cases[3].diff.patch(cases[3].A),    cases[3].B);
  ASSERT_EQ(cases[3].diff.uniPatch(cases[3].A), cases[3].B);
}

TEST_F (PatchTest, test4) {
  ASSERT_EQ(cases[4].diff.patch(cases[4].A),    cases[4].B);
  ASSERT_EQ(cases[4].diff.uniPatch(cases[4].A), cases[4].B);
}

TEST_F (PatchTest, test5) {
  ASSERT_EQ(cases[5].diff.patch(cases[5].A),    cases[5].B);
  ASSERT_EQ(cases[5].diff.uniPatch(cases[5].A), cases[5].B);
}

TEST_F (PatchTest, test6) {
  ASSERT_EQ(cases[6].diff.patch(cases[6].A),    cases[6].B);
  ASSERT_EQ(cases[6].diff.uniPatch(cases[6].A), cases[6].B);
}

TEST_F (PatchTest, test7) {
  ASSERT_EQ(cases[7].diff.patch(cases[7].A),    cases[7].B);
  ASSERT_EQ(cases[7].diff.uniPatch(cases[7].A), cases[7].B);
}

int main (int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}





