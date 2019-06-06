//
// Created by Xun Li on 5/30/19.
//

#include <limits.h>
#include <libgeoda.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

namespace {

    TEST(LayerFunctions, getNumObs) {
        GeoDa gda("../data/natregimes.shp");
        EXPECT_THAT(gda.getNumObs(), 3085);
    }

    TEST(LayerFunctions, getNumCols) {
        GeoDa gda("../data/natregimes.shp");
        EXPECT_THAT(gda.getNumCols(), 12);
    }
}

