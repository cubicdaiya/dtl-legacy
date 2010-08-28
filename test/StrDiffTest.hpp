#ifndef STRDIFF_TEST
#define STRDIFF_TEST

#include "dtl_test_common.hpp"
#include "comparators.hpp"

class StrDiffTest : public ::testing::Test
{
public :
protected :
    typedef char elem;
    typedef string sequence;
    typedef pair< elem, elemInfo > sesElem;
    typedef vector< elem > elemVec;
    typedef vector< sesElem > sesElemVec;
    typedef vector< uniHunk< sesElem > > uniHunkVec;
    typedef struct case_t {
        sequence A;
        sequence B;
        size_t editdis;
        elemVec lcs_v;
        sequence lcs_s;
        sesElemVec ses_seq;
        uniHunkVec hunk_v;
    } case_t;
    typedef vector< case_t > caseVec;
    
    caseVec diff_cases;
    caseVec only_editdis_cases;
    caseVec custom_cases;
    
    template < typename comparator >
    case_t createCase (sequence a, sequence b, bool onlyEditdis = false) {
        case_t c;
        comparator comp;
        c.A = a;
        c.B = b;
        Diff< elem, sequence, comparator > diff(a, b, comp);
        if (onlyEditdis) {
            diff.onOnlyEditDistance();
        }
        diff.compose();
        diff.composeUnifiedHunks();
        c.editdis     = diff.getEditDistance();
        elemVec lcs_v = diff.getLcsVec();
        c.lcs_s       = sequence(lcs_v.begin(), lcs_v.end());
        c.ses_seq     = diff.getSes().getSequence();
        c.hunk_v      = diff.getUniHunks();
        diff.printSES();
        return c;
    }
    
    void SetUp() {
        diff_cases.push_back(createCase< Compare< elem > >("abc",               "abd"));                       // 0
        diff_cases.push_back(createCase< Compare< elem > >("acbdeacbed",        "acebdabbabed"));              // 1
        diff_cases.push_back(createCase< Compare< elem > >("abcdef",            "dacfea"));                    // 2
        diff_cases.push_back(createCase< Compare< elem > >("abcbda",            "bdcaba"));                    // 3
        diff_cases.push_back(createCase< Compare< elem > >("bokko",             "bokkko"));                    // 4
        diff_cases.push_back(createCase< Compare< elem > >("",                  ""));                          // 5
        diff_cases.push_back(createCase< Compare< elem > >("a",                 ""));                          // 6
        diff_cases.push_back(createCase< Compare< elem > >("",                  "b"));                         // 7
        diff_cases.push_back(createCase< Compare< elem > >("acbdeaqqqqqqqcbed", "acebdabbqqqqqqqabed"));       // 8
        
        only_editdis_cases.push_back(createCase< Compare< elem > >("abc",                "abd",                 true)); // 0
        only_editdis_cases.push_back(createCase< Compare< elem > >("acbdeacbed",         "acebdabbabed",        true)); // 1
        only_editdis_cases.push_back(createCase< Compare< elem > >("abcdef",             "dacfea",              true)); // 2
        only_editdis_cases.push_back(createCase< Compare< elem > >("abcbda",             "bdcaba",              true)); // 3
        only_editdis_cases.push_back(createCase< Compare< elem > >("bokko",              "bokkko",              true)); // 4
        only_editdis_cases.push_back(createCase< Compare< elem > >("",                   "",                    true)); // 5
        only_editdis_cases.push_back(createCase< Compare< elem > >("a",                  "",                    true)); // 6
        only_editdis_cases.push_back(createCase< Compare< elem > >("",                   "b",                   true)); // 7
        only_editdis_cases.push_back(createCase< Compare< elem > >("acbdeaqqqqqqqcbed",  "acebdabbqqqqqqqabed", true)); // 8

        custom_cases.push_back(createCase< CaseInsensitive >("abc", "Abc")); // 0
    }
    
    void TearDown () {}
    
};

/**
 * StrDiffTest
 * check list is following
 * - editdistance
 * - LCS
 * - SES
 * - Unified Format Difference
 * - onOnlyEditDistance
 */

TEST_F (StrDiffTest, diff_test0) {
    
    EXPECT_EQ(2,          diff_cases[0].editdis);
    
    EXPECT_EQ("ab",       diff_cases[0].lcs_s);
    
    ASSERT_EQ('a',        diff_cases[0].ses_seq[0].first);
    ASSERT_EQ('b',        diff_cases[0].ses_seq[1].first);
    ASSERT_EQ('c',        diff_cases[0].ses_seq[2].first);
    ASSERT_EQ('d',        diff_cases[0].ses_seq[3].first);
    ASSERT_EQ(SES_COMMON, diff_cases[0].ses_seq[0].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[0].ses_seq[1].second.type);
    ASSERT_EQ(SES_DELETE, diff_cases[0].ses_seq[2].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[0].ses_seq[3].second.type);
    
    ASSERT_EQ(1,          diff_cases[0].hunk_v[0].a);
    ASSERT_EQ(3,          diff_cases[0].hunk_v[0].b);
    ASSERT_EQ(1,          diff_cases[0].hunk_v[0].c);
    ASSERT_EQ(3,          diff_cases[0].hunk_v[0].d);
    ASSERT_EQ('a',        diff_cases[0].hunk_v[0].common[0][0].first);
    ASSERT_EQ('b',        diff_cases[0].hunk_v[0].common[0][1].first);
    ASSERT_EQ('c',        diff_cases[0].hunk_v[0].change[0].first);
    ASSERT_EQ('d',        diff_cases[0].hunk_v[0].change[1].first);
    ASSERT_EQ(SES_COMMON, diff_cases[0].hunk_v[0].common[0][0].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[0].hunk_v[0].common[0][1].second.type);
    ASSERT_EQ(SES_DELETE, diff_cases[0].hunk_v[0].change[0].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[0].hunk_v[0].change[1].second.type);
    ASSERT_TRUE(diff_cases[0].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test1) {
    EXPECT_EQ(6,          diff_cases[1].editdis);
    
    EXPECT_EQ("acbdabed", diff_cases[1].lcs_s);
    
    ASSERT_EQ('a',        diff_cases[1].ses_seq[0].first);
    ASSERT_EQ('c',        diff_cases[1].ses_seq[1].first);
    ASSERT_EQ('e',        diff_cases[1].ses_seq[2].first);
    ASSERT_EQ('b',        diff_cases[1].ses_seq[3].first);
    ASSERT_EQ('d',        diff_cases[1].ses_seq[4].first);
    ASSERT_EQ('e',        diff_cases[1].ses_seq[5].first);
    ASSERT_EQ('a',        diff_cases[1].ses_seq[6].first);
    ASSERT_EQ('c',        diff_cases[1].ses_seq[7].first);
    ASSERT_EQ('b',        diff_cases[1].ses_seq[8].first);
    ASSERT_EQ('b',        diff_cases[1].ses_seq[9].first);
    ASSERT_EQ('a',        diff_cases[1].ses_seq[10].first);
    ASSERT_EQ('b',        diff_cases[1].ses_seq[11].first);
    ASSERT_EQ('e',        diff_cases[1].ses_seq[12].first);
    ASSERT_EQ('d',        diff_cases[1].ses_seq[13].first);
    ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[0].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[1].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[1].ses_seq[2].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[3].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[4].second.type);
    ASSERT_EQ(SES_DELETE, diff_cases[1].ses_seq[5].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[6].second.type);
    ASSERT_EQ(SES_DELETE, diff_cases[1].ses_seq[7].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[8].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[1].ses_seq[9].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[1].ses_seq[10].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[1].ses_seq[11].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[12].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[1].ses_seq[13].second.type);
    
    ASSERT_EQ(1,          diff_cases[1].hunk_v[0].a);
    ASSERT_EQ(10,         diff_cases[1].hunk_v[0].b);
    ASSERT_EQ(1,          diff_cases[1].hunk_v[0].c);
    ASSERT_EQ(12,         diff_cases[1].hunk_v[0].d);
    ASSERT_EQ('a',        diff_cases[1].hunk_v[0].common[0][0].first);
    ASSERT_EQ('c',        diff_cases[1].hunk_v[0].common[0][1].first);
    ASSERT_EQ('e',        diff_cases[1].hunk_v[0].change[0].first);
    ASSERT_EQ('b',        diff_cases[1].hunk_v[0].change[1].first);
    ASSERT_EQ('d',        diff_cases[1].hunk_v[0].change[2].first);
    ASSERT_EQ('e',        diff_cases[1].hunk_v[0].change[3].first);
    ASSERT_EQ('a',        diff_cases[1].hunk_v[0].change[4].first);
    ASSERT_EQ('c',        diff_cases[1].hunk_v[0].change[5].first);
    ASSERT_EQ('b',        diff_cases[1].hunk_v[0].change[6].first);
    ASSERT_EQ('b',        diff_cases[1].hunk_v[0].change[7].first);
    ASSERT_EQ('a',        diff_cases[1].hunk_v[0].change[8].first);
    ASSERT_EQ('b',        diff_cases[1].hunk_v[0].change[9].first);
    ASSERT_EQ('e',        diff_cases[1].hunk_v[0].change[10].first);
    ASSERT_EQ('d',        diff_cases[1].hunk_v[0].change[11].first);
    ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].common[0][0].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].common[0][1].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[1].hunk_v[0].change[0].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].change[1].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].change[2].second.type);
    ASSERT_EQ(SES_DELETE, diff_cases[1].hunk_v[0].change[3].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].change[4].second.type);
    ASSERT_EQ(SES_DELETE, diff_cases[1].hunk_v[0].change[5].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].change[6].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[1].hunk_v[0].change[7].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[1].hunk_v[0].change[8].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[1].hunk_v[0].change[9].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].change[10].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[1].hunk_v[0].change[11].second.type);
    ASSERT_TRUE(diff_cases[1].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test2) {
    EXPECT_EQ(6,          diff_cases[2].editdis);
    
    EXPECT_EQ("acf",      diff_cases[2].lcs_s);
    
    ASSERT_EQ('d',        diff_cases[2].ses_seq[0].first);
    ASSERT_EQ('a',        diff_cases[2].ses_seq[1].first);
    ASSERT_EQ('b',        diff_cases[2].ses_seq[2].first);
    ASSERT_EQ('c',        diff_cases[2].ses_seq[3].first);
    ASSERT_EQ('d',        diff_cases[2].ses_seq[4].first);
    ASSERT_EQ('e',        diff_cases[2].ses_seq[5].first);
    ASSERT_EQ('f',        diff_cases[2].ses_seq[6].first);
    ASSERT_EQ('e',        diff_cases[2].ses_seq[7].first);
    ASSERT_EQ('a',        diff_cases[2].ses_seq[8].first);
    ASSERT_EQ(SES_ADD,    diff_cases[2].ses_seq[0].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[2].ses_seq[1].second.type);
    ASSERT_EQ(SES_DELETE, diff_cases[2].ses_seq[2].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[2].ses_seq[3].second.type);
    ASSERT_EQ(SES_DELETE, diff_cases[2].ses_seq[4].second.type);
    ASSERT_EQ(SES_DELETE, diff_cases[2].ses_seq[5].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[2].ses_seq[6].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[2].ses_seq[7].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[2].ses_seq[8].second.type);
    
    ASSERT_EQ(1,          diff_cases[2].hunk_v[0].a);
    ASSERT_EQ(6,          diff_cases[2].hunk_v[0].b);
    ASSERT_EQ(1,          diff_cases[2].hunk_v[0].c);
    ASSERT_EQ(6,          diff_cases[2].hunk_v[0].d);
    ASSERT_EQ('d',        diff_cases[2].hunk_v[0].change[0].first);
    ASSERT_EQ('a',        diff_cases[2].hunk_v[0].change[1].first);
    ASSERT_EQ('b',        diff_cases[2].hunk_v[0].change[2].first);
    ASSERT_EQ('c',        diff_cases[2].hunk_v[0].change[3].first);
    ASSERT_EQ('d',        diff_cases[2].hunk_v[0].change[4].first);
    ASSERT_EQ('e',        diff_cases[2].hunk_v[0].change[5].first);
    ASSERT_EQ('f',        diff_cases[2].hunk_v[0].change[6].first);
    ASSERT_EQ('e',        diff_cases[2].hunk_v[0].change[7].first);
    ASSERT_EQ('a',        diff_cases[2].hunk_v[0].change[8].first);
    ASSERT_TRUE(diff_cases[2].hunk_v[0].common[0].empty());
    ASSERT_EQ(SES_ADD,    diff_cases[2].hunk_v[0].change[0].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[2].hunk_v[0].change[1].second.type);
    ASSERT_EQ(SES_DELETE, diff_cases[2].hunk_v[0].change[2].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[2].hunk_v[0].change[3].second.type);
    ASSERT_EQ(SES_DELETE, diff_cases[2].hunk_v[0].change[4].second.type);
    ASSERT_EQ(SES_DELETE, diff_cases[2].hunk_v[0].change[5].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[2].hunk_v[0].change[6].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[2].hunk_v[0].change[7].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[2].hunk_v[0].change[8].second.type);
    ASSERT_TRUE(diff_cases[2].hunk_v[0].common[1].empty());
    
}

TEST_F (StrDiffTest, diff_test3) {
    EXPECT_EQ(4,          diff_cases[3].editdis);
    
    EXPECT_EQ("bcba",     diff_cases[3].lcs_s);
    
    ASSERT_EQ('a',        diff_cases[3].ses_seq[0].first);
    ASSERT_EQ('b',        diff_cases[3].ses_seq[1].first);
    ASSERT_EQ('d',        diff_cases[3].ses_seq[2].first);
    ASSERT_EQ('c',        diff_cases[3].ses_seq[3].first);
    ASSERT_EQ('a',        diff_cases[3].ses_seq[4].first);
    ASSERT_EQ('b',        diff_cases[3].ses_seq[5].first);
    ASSERT_EQ('d',        diff_cases[3].ses_seq[6].first);
    ASSERT_EQ('a',        diff_cases[3].ses_seq[7].first);
    ASSERT_EQ(SES_DELETE, diff_cases[3].ses_seq[0].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[3].ses_seq[1].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[3].ses_seq[2].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[3].ses_seq[3].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[3].ses_seq[4].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[3].ses_seq[5].second.type);
    ASSERT_EQ(SES_DELETE, diff_cases[3].ses_seq[6].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[3].ses_seq[7].second.type);
    
    ASSERT_EQ(1,          diff_cases[3].hunk_v[0].a);
    ASSERT_EQ(6,          diff_cases[3].hunk_v[0].b);
    ASSERT_EQ(1,          diff_cases[3].hunk_v[0].c);
    ASSERT_EQ(6,          diff_cases[3].hunk_v[0].d);
    ASSERT_TRUE(diff_cases[3].hunk_v[0].common[0].empty());
    ASSERT_EQ('a',        diff_cases[3].hunk_v[0].change[0].first);
    ASSERT_EQ('b',        diff_cases[3].hunk_v[0].change[1].first);
    ASSERT_EQ('d',        diff_cases[3].hunk_v[0].change[2].first);
    ASSERT_EQ('c',        diff_cases[3].hunk_v[0].change[3].first);
    ASSERT_EQ('a',        diff_cases[3].hunk_v[0].change[4].first);
    ASSERT_EQ('b',        diff_cases[3].hunk_v[0].change[5].first);
    ASSERT_EQ('d',        diff_cases[3].hunk_v[0].change[6].first);
    ASSERT_EQ('a',        diff_cases[3].hunk_v[0].change[7].first);
    ASSERT_EQ(SES_DELETE, diff_cases[3].hunk_v[0].change[0].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[3].hunk_v[0].change[1].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[3].hunk_v[0].change[2].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[3].hunk_v[0].change[3].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[3].hunk_v[0].change[4].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[3].hunk_v[0].change[5].second.type);
    ASSERT_EQ(SES_DELETE, diff_cases[3].hunk_v[0].change[6].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[3].hunk_v[0].change[7].second.type);
    ASSERT_TRUE(diff_cases[3].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test4) {
    EXPECT_EQ(1,          diff_cases[4].editdis);
    
    EXPECT_EQ("bokko",    diff_cases[4].lcs_s);
    
    ASSERT_EQ('b',        diff_cases[4].ses_seq[0].first);
    ASSERT_EQ('o',        diff_cases[4].ses_seq[1].first);
    ASSERT_EQ('k',        diff_cases[4].ses_seq[2].first);
    ASSERT_EQ('k',        diff_cases[4].ses_seq[3].first);
    ASSERT_EQ('k',        diff_cases[4].ses_seq[4].first);
    ASSERT_EQ('o',        diff_cases[4].ses_seq[5].first);
    ASSERT_EQ(SES_COMMON, diff_cases[4].ses_seq[0].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[4].ses_seq[1].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[4].ses_seq[2].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[4].ses_seq[3].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[4].ses_seq[4].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[4].ses_seq[5].second.type);
    
    ASSERT_EQ(2,          diff_cases[4].hunk_v[0].a);
    ASSERT_EQ(4,          diff_cases[4].hunk_v[0].b);
    ASSERT_EQ(2,          diff_cases[4].hunk_v[0].c);
    ASSERT_EQ(5,          diff_cases[4].hunk_v[0].d);
    ASSERT_EQ('o',        diff_cases[4].hunk_v[0].common[0][0].first);
    ASSERT_EQ('k',        diff_cases[4].hunk_v[0].common[0][1].first);
    ASSERT_EQ('k',        diff_cases[4].hunk_v[0].common[0][2].first);
    ASSERT_EQ('k',        diff_cases[4].hunk_v[0].change[0].first);
    ASSERT_EQ('o',        diff_cases[4].hunk_v[0].change[1].first);
    ASSERT_EQ(SES_COMMON, diff_cases[4].hunk_v[0].common[0][0].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[4].hunk_v[0].common[0][1].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[4].hunk_v[0].common[0][2].second.type);
    ASSERT_EQ(SES_ADD,    diff_cases[4].hunk_v[0].change[0].second.type);
    ASSERT_EQ(SES_COMMON, diff_cases[4].hunk_v[0].change[1].second.type);
    ASSERT_TRUE(diff_cases[4].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test5) {
    EXPECT_EQ(0,  diff_cases[5].editdis);
    
    EXPECT_EQ("", diff_cases[5].lcs_s);
    
    ASSERT_TRUE(diff_cases[5].ses_seq.empty());
    
    ASSERT_TRUE(diff_cases[5].hunk_v.empty());
}

TEST_F (StrDiffTest, diff_test6) {
    EXPECT_EQ(1,          diff_cases[6].editdis);
    
    EXPECT_EQ("",         diff_cases[6].lcs_s);
    
    ASSERT_EQ('a',        diff_cases[6].ses_seq[0].first);
    ASSERT_EQ(SES_DELETE, diff_cases[6].ses_seq[0].second.type);
    
    ASSERT_EQ(1,          diff_cases[6].hunk_v[0].a);
    ASSERT_EQ(1,          diff_cases[6].hunk_v[0].b);
    ASSERT_EQ(1,          diff_cases[6].hunk_v[0].c);
    ASSERT_EQ(0,          diff_cases[6].hunk_v[0].d);
    ASSERT_TRUE(diff_cases[6].hunk_v[0].common[0].empty());
    ASSERT_EQ('a',        diff_cases[6].hunk_v[0].change[0].first);
    ASSERT_EQ(SES_DELETE, diff_cases[6].hunk_v[0].change[0].second.type);
    ASSERT_TRUE(diff_cases[6].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test7) {
    EXPECT_EQ(1,       diff_cases[7].editdis);
    
    EXPECT_EQ("",      diff_cases[7].lcs_s);
    
    ASSERT_EQ('b',     diff_cases[7].ses_seq[0].first);
    ASSERT_EQ(SES_ADD, diff_cases[7].ses_seq[0].second.type);
    
    ASSERT_EQ(1,       diff_cases[7].hunk_v[0].a);
    ASSERT_EQ(0,       diff_cases[7].hunk_v[0].b);
    ASSERT_EQ(1,       diff_cases[7].hunk_v[0].c);
    ASSERT_EQ(1,       diff_cases[7].hunk_v[0].d);
    ASSERT_TRUE(diff_cases[7].hunk_v[0].common[0].empty());
    ASSERT_EQ('b',     diff_cases[7].hunk_v[0].change[0].first);
    ASSERT_EQ(SES_ADD, diff_cases[7].hunk_v[0].change[0].second.type);
    ASSERT_TRUE(diff_cases[7].hunk_v[0].common[1].empty());
}

TEST_F (StrDiffTest, diff_test8) {
    EXPECT_EQ(6,                 diff_cases[8].editdis);
    
    EXPECT_EQ("acbdaqqqqqqqbed", diff_cases[8].lcs_s);
    
    ASSERT_EQ('a',               diff_cases[8].ses_seq[0].first);
    ASSERT_EQ('c',               diff_cases[8].ses_seq[1].first);
    ASSERT_EQ('e',               diff_cases[8].ses_seq[2].first);
    ASSERT_EQ('b',               diff_cases[8].ses_seq[3].first);
    ASSERT_EQ('d',               diff_cases[8].ses_seq[4].first);
    ASSERT_EQ('e',               diff_cases[8].ses_seq[5].first);
    ASSERT_EQ('a',               diff_cases[8].ses_seq[6].first);
    ASSERT_EQ('b',               diff_cases[8].ses_seq[7].first);
    ASSERT_EQ('b',               diff_cases[8].ses_seq[8].first);
    ASSERT_EQ('q',               diff_cases[8].ses_seq[9].first);
    ASSERT_EQ('q',               diff_cases[8].ses_seq[10].first);
    ASSERT_EQ('q',               diff_cases[8].ses_seq[11].first);
    ASSERT_EQ('q',               diff_cases[8].ses_seq[12].first);
    ASSERT_EQ('q',               diff_cases[8].ses_seq[13].first);
    ASSERT_EQ('q',               diff_cases[8].ses_seq[14].first);
    ASSERT_EQ('q',               diff_cases[8].ses_seq[15].first);
    ASSERT_EQ('a',               diff_cases[8].ses_seq[16].first);
    ASSERT_EQ('c',               diff_cases[8].ses_seq[17].first);
    ASSERT_EQ('b',               diff_cases[8].ses_seq[18].first);
    ASSERT_EQ('e',               diff_cases[8].ses_seq[19].first);
    ASSERT_EQ('d',               diff_cases[8].ses_seq[20].first);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[0].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[1].second.type);
    ASSERT_EQ(SES_ADD,           diff_cases[8].ses_seq[2].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[3].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[4].second.type);
    ASSERT_EQ(SES_DELETE,        diff_cases[8].ses_seq[5].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[6].second.type);
    ASSERT_EQ(SES_ADD,           diff_cases[8].ses_seq[7].second.type);
    ASSERT_EQ(SES_ADD,           diff_cases[8].ses_seq[8].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[9].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[10].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[11].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[12].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[13].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[14].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[15].second.type);
    ASSERT_EQ(SES_ADD,           diff_cases[8].ses_seq[16].second.type);
    ASSERT_EQ(SES_DELETE,        diff_cases[8].ses_seq[17].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[18].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[19].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].ses_seq[20].second.type);
    
    ASSERT_EQ(1,                 diff_cases[8].hunk_v[0].a);
    ASSERT_EQ(9,                 diff_cases[8].hunk_v[0].b);
    ASSERT_EQ(1,                 diff_cases[8].hunk_v[0].c);
    ASSERT_EQ(11,                diff_cases[8].hunk_v[0].d);
    ASSERT_EQ(11,                diff_cases[8].hunk_v[1].a);
    ASSERT_EQ(7,                 diff_cases[8].hunk_v[1].b);
    ASSERT_EQ(13,                diff_cases[8].hunk_v[1].c);
    ASSERT_EQ(7,                 diff_cases[8].hunk_v[1].d);
    
    ASSERT_EQ('a',               diff_cases[8].hunk_v[0].common[0][0].first);
    ASSERT_EQ('c',               diff_cases[8].hunk_v[0].common[0][1].first);
    ASSERT_EQ('e',               diff_cases[8].hunk_v[0].change[0].first);
    ASSERT_EQ('b',               diff_cases[8].hunk_v[0].change[1].first);
    ASSERT_EQ('d',               diff_cases[8].hunk_v[0].change[2].first);
    ASSERT_EQ('e',               diff_cases[8].hunk_v[0].change[3].first);
    ASSERT_EQ('a',               diff_cases[8].hunk_v[0].change[4].first);
    ASSERT_EQ('b',               diff_cases[8].hunk_v[0].change[5].first);
    ASSERT_EQ('b',               diff_cases[8].hunk_v[0].change[6].first);
    ASSERT_EQ('q',               diff_cases[8].hunk_v[0].change[7].first);
    ASSERT_EQ('q',               diff_cases[8].hunk_v[0].change[8].first);
    ASSERT_EQ('q',               diff_cases[8].hunk_v[0].change[9].first);
    ASSERT_EQ('q',               diff_cases[8].hunk_v[1].common[0][0].first);
    ASSERT_EQ('q',               diff_cases[8].hunk_v[1].common[0][1].first);
    ASSERT_EQ('q',               diff_cases[8].hunk_v[1].common[0][2].first);
    ASSERT_EQ('c',               diff_cases[8].hunk_v[1].change[0].first);
    ASSERT_EQ('a',               diff_cases[8].hunk_v[1].change[1].first);
    ASSERT_EQ('b',               diff_cases[8].hunk_v[1].change[2].first);
    ASSERT_EQ('e',               diff_cases[8].hunk_v[1].change[3].first);
    ASSERT_EQ('d',               diff_cases[8].hunk_v[1].change[4].first);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].hunk_v[0].common[0][0].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].hunk_v[0].common[0][1].second.type);
    ASSERT_EQ(SES_ADD,           diff_cases[8].hunk_v[0].change[0].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].hunk_v[0].change[1].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].hunk_v[0].change[2].second.type);
    ASSERT_EQ(SES_DELETE,        diff_cases[8].hunk_v[0].change[3].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].hunk_v[0].change[4].second.type);
    ASSERT_EQ(SES_ADD,           diff_cases[8].hunk_v[0].change[5].second.type);
    ASSERT_EQ(SES_ADD,           diff_cases[8].hunk_v[0].change[6].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].hunk_v[0].change[7].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].hunk_v[0].change[8].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].hunk_v[0].change[9].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].hunk_v[1].common[0][0].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].hunk_v[1].common[0][1].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].hunk_v[1].common[0][2].second.type);
    ASSERT_EQ(SES_DELETE,        diff_cases[8].hunk_v[1].change[0].second.type);
    ASSERT_EQ(SES_ADD,           diff_cases[8].hunk_v[1].change[1].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].hunk_v[1].change[2].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].hunk_v[1].change[3].second.type);
    ASSERT_EQ(SES_COMMON,        diff_cases[8].hunk_v[1].change[4].second.type);
}

TEST_F (StrDiffTest, only_editdis_test0) {
    EXPECT_EQ(2,       only_editdis_cases[0].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[0].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[0].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[0].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test1) {
    EXPECT_EQ(6,       only_editdis_cases[1].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[1].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[1].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[1].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test2) {
    EXPECT_EQ(6,       only_editdis_cases[2].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[2].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[2].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[2].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test3) {
    EXPECT_EQ(4,       only_editdis_cases[3].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[3].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[3].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[3].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test4) {
    EXPECT_EQ(1,       only_editdis_cases[4].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[4].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[4].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[4].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test5) {
    EXPECT_EQ(0,       only_editdis_cases[5].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[5].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[5].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[5].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test6) {
    EXPECT_EQ(1,       only_editdis_cases[6].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[6].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[6].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[6].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test7) {
    EXPECT_EQ(1,       only_editdis_cases[7].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[7].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[7].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[7].hunk_v.empty());
}

TEST_F (StrDiffTest, only_editdis_test8) {
    EXPECT_EQ(6,       only_editdis_cases[8].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[8].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[8].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[8].hunk_v.empty());
}

TEST_F (StrDiffTest, custom_comparator_test0) {
    EXPECT_EQ(0,     custom_cases[0].editdis);
    
    EXPECT_EQ("Abc", custom_cases[0].lcs_s);

    ASSERT_EQ('A',   custom_cases[0].ses_seq[0].first);
    ASSERT_EQ('b',   custom_cases[0].ses_seq[1].first);
    ASSERT_EQ('c',   custom_cases[0].ses_seq[2].first);
    
    ASSERT_TRUE(custom_cases[0].hunk_v.empty());
}


#endif // STRDIFF_TEST
