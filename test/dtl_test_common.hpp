
#ifndef DTL_TEST_COMMON
#define DTL_TEST_COMMON

#include <gtest/gtest.h>
#include <cstdio>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <dtl/dtl.hpp>

using std::string;
using std::vector;
using std::pair;
using std::ifstream;
using std::ofstream;

using dtl::Diff;
using dtl::Diff3;
using dtl::Compare;
using dtl::SES_COMMON;
using dtl::SES_ADD;
using dtl::SES_DELETE;
using dtl::elemInfo;
using dtl::uniHunk;

#define dtl_test_typedefs(e_type, seq_type)                         \
    typedef e_type                       elem;                      \
    typedef seq_type                     sequence;                  \
    typedef pair< elem, elemInfo >       sesElem;                   \
    typedef vector< elem >               elemVec;                   \
    typedef vector< sesElem >            sesElemVec;                \
    typedef vector< uniHunk< sesElem > > uniHunkVec;
    

#endif // DTL_TEST_COMMON
