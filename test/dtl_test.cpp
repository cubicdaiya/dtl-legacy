/**
 * It is necessary to use googletest to run tests.
 */

#include <gtest/gtest.h>
#include "StrDiffTest.hpp"
#include "IntDiffTest.hpp"
#include "StrDiff3Test.hpp"
#include "PatchTest.hpp"

int main (int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}





