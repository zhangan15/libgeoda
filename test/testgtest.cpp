//
// Created by Xun Li on 5/30/19.
//

#include <limits.h>
#include <libgeoda.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

namespace {

    TEST(LayerFunctions, memory) {
        std::string ds = "memory";
        GeoDa gda(ds);
        EXPECT_THAT(gda.GetNumObs(), 3085);
    }

    TEST(LayerFunctions, getNumObs) {
        GeoDa gda("../data/natregimes.shp");
        EXPECT_THAT(gda.GetNumObs(), 3085);
    }

    TEST(LayerFunctions, getNumCols) {
        GeoDa gda("../data/natregimes.shp");
        EXPECT_THAT(gda.GetNumCols(), 12);
    }
}

