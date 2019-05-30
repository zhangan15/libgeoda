//
// Created by Xun Li on 5/30/19.
//

#include <limits.h>
#include <library.h>
#include <gtest/gtest.h>

namespace {
    TEST(FactorialTest, Zero) {
        EXPECT_EQ(1, Factorial(0));
    }
}