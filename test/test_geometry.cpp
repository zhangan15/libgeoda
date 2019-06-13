//
// Created by Xun Li on 2019-06-12.
//

#include <limits.h>
#include <libgeoda.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string.h>

using namespace testing;

namespace {

    TEST(GeomFunctions, wkb) {
        GeoDa gda("../data/natregimes.shp");
        char* wkb = gda.GetGeometryWKB(2);
        size_t len = strlen((char*)wkb);
        EXPECT_THAT(gda.GetNumObs(), 3085);
    }
}