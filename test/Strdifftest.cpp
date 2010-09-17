#include "dtl_test_common.hpp"
#include "comparators.hpp"

class Strdifftest : public ::testing::Test
{
protected :
    dtl_test_typedefs(char, string)
    typedef struct case_t {
        sequence   A;
        sequence   B;
        size_t     editdis;
        elemVec    lcs_v;
        sequence   lcs_s;
        sesElemVec ses_seq;
        uniHunkVec hunk_v;
        size_t     editdis_ses;
        size_t     editdis_uni;
        string     path_rses;
        string     path_rhunks;
    } case_t;
    typedef vector< case_t > caseVec;
    
    enum type_diff { TYPE_DIFF_SES, TYPE_DIFF_UNI };
    
    caseVec diff_cases;
    caseVec only_editdis_cases;
    caseVec custom_cases;
    
    template < typename comparator >
    case_t createCase (const sequence a, const sequence b, string test_name, bool onlyEditdis = false) {
        case_t  c;
        elemVec lcs_v;

        Diff< elem, sequence, comparator > diff(a, b);
        if (onlyEditdis) {
            diff.onOnlyEditDistance();
        }

        diff.compose();
        diff.composeUnifiedHunks();
        lcs_v = diff.getLcsVec();

        if (test_name != "") {
            string path_lses   = create_path(test_name, TYPE_DIFF_SES);
            string path_rses   = create_path(test_name, TYPE_DIFF_SES, true);
            string path_lhunks = create_path(test_name, TYPE_DIFF_UNI);
            string path_rhunks = create_path(test_name, TYPE_DIFF_UNI, true);
            
            create_file< comparator >(path_rses,   diff, TYPE_DIFF_SES);
            create_file< comparator >(path_rhunks, diff, TYPE_DIFF_UNI);
            
            c.editdis_ses = cal_diff_uni(path_lses,   path_rses);
            c.editdis_uni = cal_diff_uni(path_lhunks, path_rhunks);
            c.path_rses   = path_rses;
            c.path_rhunks = path_rhunks;
        }

        c.A       = a;
        c.B       = b;
        c.editdis = diff.getEditDistance();
        c.lcs_s   = sequence(lcs_v.begin(), lcs_v.end());
        c.ses_seq = diff.getSes().getSequence();
        c.hunk_v  = diff.getUniHunks();
        
        return c;
    }
    
    void SetUp(void) {
        diff_cases.push_back(createCase< Compare< elem > >("abc",               "abd",                 "diff_test0"));
        diff_cases.push_back(createCase< Compare< elem > >("acbdeacbed",        "acebdabbabed",        "diff_test1"));
        diff_cases.push_back(createCase< Compare< elem > >("abcdef",            "dacfea",              "diff_test2"));
        diff_cases.push_back(createCase< Compare< elem > >("abcbda",            "bdcaba",              "diff_test3"));
        diff_cases.push_back(createCase< Compare< elem > >("bokko",             "bokkko",              "diff_test4"));
        diff_cases.push_back(createCase< Compare< elem > >("",                  "",                    "diff_test5"));
        diff_cases.push_back(createCase< Compare< elem > >("a",                 "",                    "diff_test6"));
        diff_cases.push_back(createCase< Compare< elem > >("",                  "b",                   "diff_test7"));
        diff_cases.push_back(createCase< Compare< elem > >("acbdeaqqqqqqqcbed", "acebdabbqqqqqqqabed", "diff_test8"));
        
        only_editdis_cases.push_back(createCase< Compare< elem > >("abc",                "abd",                 "", true));
        only_editdis_cases.push_back(createCase< Compare< elem > >("acbdeacbed",         "acebdabbabed",        "", true));
        only_editdis_cases.push_back(createCase< Compare< elem > >("abcdef",             "dacfea",              "", true));
        only_editdis_cases.push_back(createCase< Compare< elem > >("abcbda",             "bdcaba",              "", true));
        only_editdis_cases.push_back(createCase< Compare< elem > >("bokko",              "bokkko",              "", true));
        only_editdis_cases.push_back(createCase< Compare< elem > >("",                   "",                    "", true));
        only_editdis_cases.push_back(createCase< Compare< elem > >("a",                  "",                    "", true));
        only_editdis_cases.push_back(createCase< Compare< elem > >("",                   "b",                   "", true));
        only_editdis_cases.push_back(createCase< Compare< elem > >("acbdeaqqqqqqqcbed",  "acebdabbqqqqqqqabed", "", true));
        
        custom_cases.push_back(createCase< CaseInsensitive >("abc", "Abc", ""));
    }
    
    void TearDown () {
        for_each(diff_cases.begin(), diff_cases.end(), Remover());
    }
private :

    string create_path (const string& test_name, enum type_diff t, bool is_use_suffix = false) {
        string ret;
        switch (t) {
        case TYPE_DIFF_SES:
            ret = (string("ses")   + string("/") + string("strdiff") + string("/") + test_name);
            break;
        case TYPE_DIFF_UNI:
            ret = (string("hunks") + string("/") + string("strdiff") + string("/") + test_name);
            break;
        }
        ret += is_use_suffix ? "_" : "";
        return ret;
    }
    
    template < typename comparator >
    void create_file (const string& path, Diff< elem, sequence, comparator >& diff, enum type_diff t) {
        ofstream ofs;
        ofs.open(path.c_str());
        switch (t) {
        case TYPE_DIFF_SES:
            diff.printSES(ofs);
            break;
        case TYPE_DIFF_UNI:
            diff.printUnifiedFormat(ofs);
            break;
        }
        ofs.close();
    }

    size_t cal_diff_uni (const string& path_l, const string& path_r) {
        string   buf;
        ifstream lifs(path_l.c_str());
        ifstream rifs(path_r.c_str());
        vector< string > llines;
        vector< string > rlines;
        while (getline(lifs, buf)) {
            llines.push_back(buf);
        }
        
        while (getline(rifs, buf)) {
            rlines.push_back(buf);
        }
        
        Diff< string, vector< string > > diff_uni(llines, rlines);
        diff_uni.compose();
        return diff_uni.getEditDistance();
    }
    
    class Remover {
    public :
        void operator()(const case_t& c){
            remove(c.path_rses.c_str());
            remove(c.path_rhunks.c_str());
        }
    };
    
};


/**
 * Strdifftest
 * check list is following
 * - editdistance
 * - LCS
 * - SES
 * - Unified Format Difference
 * - onOnlyEditDistance
 */

TEST_F (Strdifftest, diff_test0) {
    
    EXPECT_EQ(2,          diff_cases[0].editdis);
    
    EXPECT_EQ("ab",       diff_cases[0].lcs_s);
    
    ASSERT_EQ(0,          diff_cases[0].editdis_ses);

    ASSERT_EQ(0,          diff_cases[0].editdis_uni);
}

TEST_F (Strdifftest, diff_test1) {
    EXPECT_EQ(6,          diff_cases[1].editdis);
    
    EXPECT_EQ("acbdabed", diff_cases[1].lcs_s);
    
    ASSERT_EQ(0,          diff_cases[1].editdis_ses);
    
    ASSERT_EQ(0,          diff_cases[1].editdis_uni);
}

TEST_F (Strdifftest, diff_test2) {
    EXPECT_EQ(6,          diff_cases[2].editdis);
    
    EXPECT_EQ("acf",      diff_cases[2].lcs_s);
    
    ASSERT_EQ(0,          diff_cases[2].editdis_ses);
    
    ASSERT_EQ(0,          diff_cases[2].editdis_uni);
}

TEST_F (Strdifftest, diff_test3) {
    EXPECT_EQ(4,          diff_cases[3].editdis);
    
    EXPECT_EQ("bcba",     diff_cases[3].lcs_s);
    
    ASSERT_EQ(0,          diff_cases[3].editdis_ses);
    
    ASSERT_EQ(0,          diff_cases[3].editdis_uni);
}

TEST_F (Strdifftest, diff_test4) {
    EXPECT_EQ(1,          diff_cases[4].editdis);
    
    EXPECT_EQ("bokko",    diff_cases[4].lcs_s);
    
    ASSERT_EQ(0,          diff_cases[4].editdis_ses);
    
    ASSERT_EQ(0,          diff_cases[4].editdis_uni);
}

TEST_F (Strdifftest, diff_test5) {
    EXPECT_EQ(0,  diff_cases[5].editdis);
    
    EXPECT_EQ("", diff_cases[5].lcs_s);
    
    ASSERT_EQ(0,  diff_cases[5].editdis_ses);
    
    ASSERT_EQ(0,  diff_cases[5].editdis_uni);
}

TEST_F (Strdifftest, diff_test6) {
    EXPECT_EQ(1,          diff_cases[6].editdis);
    
    EXPECT_EQ("",         diff_cases[6].lcs_s);
    
    ASSERT_EQ(0,          diff_cases[6].editdis_ses);
    
    ASSERT_EQ(0,          diff_cases[6].editdis_uni);
}

TEST_F (Strdifftest, diff_test7) {
    EXPECT_EQ(1,       diff_cases[7].editdis);
    
    EXPECT_EQ("",      diff_cases[7].lcs_s);
    
    ASSERT_EQ(0,       diff_cases[7].editdis_ses);
    
    ASSERT_EQ(0,       diff_cases[7].editdis_uni);
}

TEST_F (Strdifftest, diff_test8) {
    EXPECT_EQ(6,                 diff_cases[8].editdis);
    
    EXPECT_EQ("acbdaqqqqqqqbed", diff_cases[8].lcs_s);
    
    ASSERT_EQ(0,                 diff_cases[8].editdis_ses);

    ASSERT_EQ(0,                 diff_cases[8].editdis_uni);
}

TEST_F (Strdifftest, only_editdis_test0) {
    EXPECT_EQ(2,       only_editdis_cases[0].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[0].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[0].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[0].hunk_v.empty());
}

TEST_F (Strdifftest, only_editdis_test1) {
    EXPECT_EQ(6,       only_editdis_cases[1].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[1].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[1].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[1].hunk_v.empty());
}

TEST_F (Strdifftest, only_editdis_test2) {
    EXPECT_EQ(6,       only_editdis_cases[2].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[2].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[2].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[2].hunk_v.empty());
}

TEST_F (Strdifftest, only_editdis_test3) {
    EXPECT_EQ(4,       only_editdis_cases[3].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[3].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[3].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[3].hunk_v.empty());
}

TEST_F (Strdifftest, only_editdis_test4) {
    EXPECT_EQ(1,       only_editdis_cases[4].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[4].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[4].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[4].hunk_v.empty());
}

TEST_F (Strdifftest, only_editdis_test5) {
    EXPECT_EQ(0,       only_editdis_cases[5].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[5].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[5].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[5].hunk_v.empty());
}

TEST_F (Strdifftest, only_editdis_test6) {
    EXPECT_EQ(1,       only_editdis_cases[6].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[6].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[6].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[6].hunk_v.empty());
}

TEST_F (Strdifftest, only_editdis_test7) {
    EXPECT_EQ(1,       only_editdis_cases[7].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[7].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[7].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[7].hunk_v.empty());
}

TEST_F (Strdifftest, only_editdis_test8) {
    EXPECT_EQ(6,       only_editdis_cases[8].editdis);
    
    EXPECT_EQ("",      only_editdis_cases[8].lcs_s);
    
    ASSERT_TRUE(only_editdis_cases[8].ses_seq.empty());
    
    ASSERT_TRUE(only_editdis_cases[8].hunk_v.empty());
}

TEST_F (Strdifftest, custom_comparator_test0) {
    EXPECT_EQ(0,     custom_cases[0].editdis);
    
    EXPECT_EQ("Abc", custom_cases[0].lcs_s);

    ASSERT_EQ('A',   custom_cases[0].ses_seq[0].first);
    ASSERT_EQ('b',   custom_cases[0].ses_seq[1].first);
    ASSERT_EQ('c',   custom_cases[0].ses_seq[2].first);
    
    ASSERT_TRUE(custom_cases[0].hunk_v.empty());
}

