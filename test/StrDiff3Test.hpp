
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
    
    caseVec merge_cases;
    caseVec detect_cases;
    caseVec specify_cases;
    
    case_t createCase (sequence a, sequence b, sequence c, sequence s) {
        case_t ct;
        ct.A = a;
        ct.B = b;
        ct.C = c;
        ct.S = s;
        ct.diff3 = Diff3< elem, sequence >(a, b, c);
        ct.diff3.compose();
        ct.diff3.setConflictSeparators('<', '|', '>');
        return ct;
    }
    
    void SetUp() {
        // merge test
        merge_cases.push_back(createCase("ab",            "b",             "bc",           "abc"));              // 0
        merge_cases.push_back(createCase("bc",            "b",             "ab",           "abc"));              // 1
        merge_cases.push_back(createCase("qqqabc",        "abc",           "abcdef",       "qqqabcdef"));        // 2
        merge_cases.push_back(createCase("abcdef",        "abc",           "qqqabc",       "qqqabcdef"));        // 3
        merge_cases.push_back(createCase("aaacccbbb",     "aaabbb",        "aaabbbqqq",    "aaacccbbbqqq"));     // 4
        merge_cases.push_back(createCase("aaabbbqqq",     "aaabbb",        "aaacccbbb",    "aaacccbbbqqq"));     // 5
        merge_cases.push_back(createCase("aeaacccbbb",    "aaabbb",        "aaabbbqqq",    "aeaacccbbbqqq"));    // 6
        merge_cases.push_back(createCase("aaabbbqqq",     "aaabbb",        "aeaacccbbb",   "aeaacccbbbqqq"));    // 7
        merge_cases.push_back(createCase("aeaacccbbb",    "aaabbb",        "aaabebbqqq",   "aeaacccbebbqqq"));   // 8
        merge_cases.push_back(createCase("aaabebbqqq",    "aaabbb",        "aeaacccbbb",   "aeaacccbebbqqq"));   // 9
        merge_cases.push_back(createCase("aaacccbbb",     "aaabbb",        "aeaabbbqqq",   "aeaacccbbbqqq"));    // 10
        merge_cases.push_back(createCase("aeaabbbqqq",    "aaabbb",        "aaacccbbb",    "aeaacccbbbqqq"));    // 11
        merge_cases.push_back(createCase("aaacccbbb",     "aaabbb",        "aaabeebbeeqqq","aaacccbeebbeeqqq")); // 12
        merge_cases.push_back(createCase("aaabeebbeeqqq", "aaabbb",        "aaacccbbb",    "aaacccbeebbeeqqq")); // 13
        merge_cases.push_back(createCase("aiueo",         "aeo",           "aeKokaki",     "aiueKokaki"));       // 14
        merge_cases.push_back(createCase("aeKokaki",      "aeo",           "aiueo",        "aiueKokaki"));       // 15
        merge_cases.push_back(createCase("1234567390",    "1234567890",    "1239567890",   "1239567390"));       // 16
        merge_cases.push_back(createCase("1239567890",    "1234567890",    "1234567390",   "1239567390"));       // 17
        merge_cases.push_back(createCase("qabcdef",       "abcdef",        "ab",           "qab"));              // 18
        merge_cases.push_back(createCase("ab",            "abcdef",        "qabcdef",      "qab"));              // 19
        merge_cases.push_back(createCase("abcdf",         "abcdef",        "acdef",        "acdf"));             // 20
        merge_cases.push_back(createCase("acdef",         "abcdef",        "abcdf",        "acdf"));             // 21
        merge_cases.push_back(createCase("acdef",         "abcdef",        "abcdfaa",      "acdfaa"));           // 22
        merge_cases.push_back(createCase("abcdfaa",       "abcdef",        "acdef",        "acdfaa"));           // 23
        
        // detect confliction test
        detect_cases.push_back(createCase("adc",           "abc",          "aec",          ""));                 // 0
        detect_cases.push_back(createCase("abqdcf",        "abcdef",       "abqqef",       ""));                 // 1
        
        // specify confliction test
        specify_cases.push_back(createCase("adc",          "abc",          "aec",          "a<d|e>c"));          // 0
        specify_cases.push_back(createCase("abqdcf",       "abcdef",       "abqqef",       "ab<qdc|qqe>f"));     // 1
        specify_cases.push_back(createCase("abqdcfzzzadc", "abcdefzzzabc", "abqqefzzzaec",
                                           "ab<qdc|qqe>fzzza<d|e>c"));                                           // 2
        specify_cases.push_back(createCase("abqdcfadc", "abcdefzzzabc",    "abqqefzezzaec",
                                           "ab<qdc|qqe>f<adc|zezzaec>"));                                        // 3
        specify_cases.push_back(createCase("adc",          "abc",          "aeczqq",       "a<d|e>czqq"));       // 4
        specify_cases.push_back(createCase("pbc",          "abc",          "qbc",          "<p|q>bc"));          // 5
        specify_cases.push_back(createCase("pdc",          "abc",          "qbc",          "<pd|qb>c"));         // 6
        specify_cases.push_back(createCase("abcdef",       "abcdefg",      "abcdefhe",     "abcdefhe"));         // 7
        
        specify_cases.push_back(createCase("abcdefqqqq",   "abcdefgzzzzzzzzzzz", "abcdefggggg", "abcdef<qqqq|ggggg>")); // 8
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
    ASSERT_TRUE(merge_cases[0].diff3.merge());
    ASSERT_EQ(merge_cases[0].S, merge_cases[0].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test1) {
    ASSERT_TRUE(merge_cases[1].diff3.merge());
    ASSERT_EQ(merge_cases[1].S, merge_cases[1].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test2) {
    ASSERT_TRUE(merge_cases[2].diff3.merge());
    ASSERT_EQ(merge_cases[2].S, merge_cases[2].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test3) {
    ASSERT_TRUE(merge_cases[3].diff3.merge());
    ASSERT_EQ(merge_cases[3].S, merge_cases[3].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test4) {
    ASSERT_TRUE(merge_cases[4].diff3.merge());
    ASSERT_EQ(merge_cases[4].S, merge_cases[4].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test5) {
    ASSERT_TRUE(merge_cases[5].diff3.merge());
    ASSERT_EQ(merge_cases[5].S, merge_cases[5].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test6) {
    ASSERT_TRUE(merge_cases[6].diff3.merge());
    ASSERT_EQ(merge_cases[6].S, merge_cases[6].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test7) {
    ASSERT_TRUE(merge_cases[7].diff3.merge());
    ASSERT_EQ(merge_cases[7].S, merge_cases[7].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test8) {
    ASSERT_TRUE(merge_cases[8].diff3.merge());
    ASSERT_EQ(merge_cases[8].S, merge_cases[8].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test9) {
    ASSERT_TRUE(merge_cases[9].diff3.merge());
    ASSERT_EQ(merge_cases[9].S, merge_cases[9].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test10) {
    ASSERT_TRUE(merge_cases[10].diff3.merge());
    ASSERT_EQ(merge_cases[10].S, merge_cases[10].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test11) {
    ASSERT_TRUE(merge_cases[11].diff3.merge());
    ASSERT_EQ(merge_cases[11].S, merge_cases[11].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test12) {
    ASSERT_TRUE(merge_cases[12].diff3.merge());
    ASSERT_EQ(merge_cases[12].S, merge_cases[12].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test13) {
    ASSERT_TRUE(merge_cases[13].diff3.merge());
    ASSERT_EQ(merge_cases[13].S, merge_cases[13].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test14) {
    ASSERT_TRUE(merge_cases[14].diff3.merge());
    ASSERT_EQ(merge_cases[14].S, merge_cases[14].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test15) {
    ASSERT_TRUE(merge_cases[15].diff3.merge());
    ASSERT_EQ(merge_cases[15].S, merge_cases[15].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test16) {
    ASSERT_TRUE(merge_cases[16].diff3.merge());
    ASSERT_EQ(merge_cases[16].S, merge_cases[16].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test17) {
    ASSERT_TRUE(merge_cases[17].diff3.merge());
    ASSERT_EQ(merge_cases[17].S, merge_cases[17].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test18) {
    ASSERT_TRUE(merge_cases[18].diff3.merge());
    ASSERT_EQ(merge_cases[18].S, merge_cases[18].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test19) {
    ASSERT_TRUE(merge_cases[19].diff3.merge());
    ASSERT_EQ(merge_cases[19].S, merge_cases[19].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test20) {
    ASSERT_TRUE(merge_cases[20].diff3.merge());
    ASSERT_EQ(merge_cases[20].S, merge_cases[20].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test21) {
    ASSERT_TRUE(merge_cases[21].diff3.merge());
    ASSERT_EQ( merge_cases[21].S, merge_cases[21].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test22) {
    ASSERT_TRUE(merge_cases[22].diff3.merge());
    ASSERT_EQ( merge_cases[22].S, merge_cases[22].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, merge_test23) {
    ASSERT_TRUE(merge_cases[23].diff3.merge());
    ASSERT_EQ(merge_cases[23].S, merge_cases[23].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, detect_confliction_test0) {
    ASSERT_FALSE(detect_cases[0].diff3.merge());
}

TEST_F (StrDiff3Test, detect_confliction_test1) {
    ASSERT_FALSE(detect_cases[1].diff3.merge());
}

TEST_F (StrDiff3Test, specify_confliction_test0) {
    ASSERT_FALSE(specify_cases[0].diff3.merge());
    ASSERT_EQ(specify_cases[0].S, specify_cases[0].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, specify_confliction_test1) {
    ASSERT_FALSE(specify_cases[1].diff3.merge());
    ASSERT_EQ(specify_cases[1].S, specify_cases[1].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, specify_confliction_test2) {
    ASSERT_FALSE(specify_cases[2].diff3.merge());
    ASSERT_EQ(specify_cases[2].S, specify_cases[2].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, specify_confliction_test3) {
    ASSERT_FALSE(specify_cases[3].diff3.merge());
    ASSERT_EQ(specify_cases[3].S, specify_cases[3].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, specify_confliction_test4) {
    ASSERT_FALSE(specify_cases[4].diff3.merge());
    ASSERT_EQ(specify_cases[4].S, specify_cases[4].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, specify_confliction_test5) {
    ASSERT_FALSE(specify_cases[5].diff3.merge());
    ASSERT_EQ(specify_cases[5].S, specify_cases[5].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, specify_confliction_test6) {
    ASSERT_FALSE(specify_cases[6].diff3.merge());
    ASSERT_EQ(specify_cases[6].S, specify_cases[6].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, specify_confliction_test7) {
    ASSERT_FALSE(specify_cases[7].diff3.merge());
    ASSERT_EQ(specify_cases[7].S, specify_cases[7].diff3.getMergedSequence());
}

TEST_F (StrDiff3Test, specify_confliction_test8) {
    ASSERT_FALSE(specify_cases[8].diff3.merge());
    ASSERT_EQ(specify_cases[8].S, specify_cases[8].diff3.getMergedSequence());
}

#endif // STRDIFF3_TEST
