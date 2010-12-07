
#include "dtl_test_common.hpp"

string create_path (const string& test_name, string diff_name, enum type_diff t, bool is_use_suffix) {
    string ret;
    switch (t) {
    case TYPE_DIFF_SES:
        ret = (string("ses")   + string("/") + diff_name + string("/") + test_name);
        break;
    case TYPE_DIFF_UNI:
        ret = (string("hunks") + string("/") + diff_name + string("/") + test_name);
        break;
    }
    ret += is_use_suffix ? "_" : "";
    return ret;
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
