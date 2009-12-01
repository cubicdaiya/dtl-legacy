
#ifndef STRDIFF3_TEST
#define STRDIFF3_TEST

#include "dtl_test_common.hpp"

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
    ct.diff3.setConflictSeparators('<', '|', '=', '>');
    return ct;
  }
  
  void SetUp() {
    // merge test
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

    // detect confliction test
    cases.push_back(createCase("adc",           "abc",        "aec",           ""));                 // 24
    cases.push_back(createCase("abqdcf",        "abcdef",     "abqqef",        ""));                 // 25

    // specify confliction test
    cases.push_back(createCase("adc",           "abc",        "aec",           "a<d|b=e>c"));        // 26
    cases.push_back(createCase("abqdcf",        "abcdef",     "abqqef",        "ab<qd|cd=qq>ef"));   // 27
    

  }
  
  void TearDown () {}
  
};

/**
 * StrDiff3Test
 * check list is following
 * - merge function
 * - detect confliction
 */
TEST_F (StrDiff3Test, merge_test0) {
  ASSERT_TRUE(cases[0].diff3.merge());
  ASSERT_EQ(cases[0].S, cases[0].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test1) {
  ASSERT_TRUE(cases[1].diff3.merge());
  ASSERT_EQ(cases[1].S, cases[1].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test2) {
  ASSERT_TRUE(cases[2].diff3.merge());
  ASSERT_EQ(cases[2].S, cases[2].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test3) {
  ASSERT_TRUE(cases[3].diff3.merge());
  ASSERT_EQ(cases[3].S, cases[3].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test4) {
  ASSERT_TRUE(cases[4].diff3.merge());
  ASSERT_EQ(cases[4].S, cases[4].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test5) {
  ASSERT_TRUE(cases[5].diff3.merge());
  ASSERT_EQ(cases[5].S, cases[5].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test6) {
  ASSERT_TRUE(cases[6].diff3.merge());
  ASSERT_EQ(cases[6].S, cases[6].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test7) {
  ASSERT_TRUE(cases[7].diff3.merge());
  ASSERT_EQ(cases[7].S, cases[7].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test8) {
  ASSERT_TRUE(cases[8].diff3.merge());
  ASSERT_EQ(cases[8].S, cases[8].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test9) {
  ASSERT_TRUE(cases[9].diff3.merge());
  ASSERT_EQ(cases[9].S, cases[9].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test10) {
  ASSERT_TRUE(cases[10].diff3.merge());
  ASSERT_EQ(cases[10].S, cases[10].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test11) {
  ASSERT_TRUE(cases[11].diff3.merge());
  ASSERT_EQ(cases[11].S, cases[11].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test12) {
  ASSERT_TRUE(cases[12].diff3.merge());
  ASSERT_EQ(cases[12].S, cases[12].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test13) {
  ASSERT_TRUE(cases[13].diff3.merge());
  ASSERT_EQ(cases[13].S, cases[13].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test14) {
  ASSERT_TRUE(cases[14].diff3.merge());
  ASSERT_EQ(cases[14].S, cases[14].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test15) {
  ASSERT_TRUE(cases[15].diff3.merge());
  ASSERT_EQ(cases[15].S, cases[15].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test16) {
  ASSERT_TRUE(cases[16].diff3.merge());
  ASSERT_EQ(cases[16].S, cases[16].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test17) {
  ASSERT_TRUE(cases[17].diff3.merge());
  ASSERT_EQ(cases[17].S, cases[17].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test18) {
  ASSERT_TRUE(cases[18].diff3.merge());
  ASSERT_EQ(cases[18].S, cases[18].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test19) {
  ASSERT_TRUE(cases[19].diff3.merge());
  ASSERT_EQ(cases[19].S, cases[19].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test20) {
  ASSERT_TRUE(cases[20].diff3.merge());
  ASSERT_EQ(cases[20].S, cases[20].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test21) {
  ASSERT_TRUE(cases[21].diff3.merge());
  ASSERT_EQ( cases[21].S, cases[21].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test22) {
  ASSERT_TRUE(cases[22].diff3.merge());
  ASSERT_EQ( cases[22].S, cases[22].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test23) {
  ASSERT_TRUE(cases[23].diff3.merge());
  ASSERT_EQ(cases[23].S, cases[23].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, detect_confliction_test0) {
  ASSERT_FALSE(cases[24].diff3.merge());
}

TEST_F (StrDiff3Test, detect_confliction_test1) {
  ASSERT_FALSE(cases[25].diff3.merge());
}

/*
TEST_F (StrDiff3Test, specify_confliction_test0) {
  ASSERT_FALSE(cases[26].diff3.merge());
  ASSERT_EQ(cases[26].S, cases[26].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, specify_confliction_test1) {
  ASSERT_FALSE(cases[27].diff3.merge());
  ASSERT_EQ(cases[27].S, cases[27].diff3.getMergedSequence());
}
*/
#endif
