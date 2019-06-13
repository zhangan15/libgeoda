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
        std::vector<std::string> wkt = gda.GetGeometryWKT();
        std::vector<std::vector<unsigned char> > wkb = gda.GetGeometryWKB();
        EXPECT_THAT(gda.GetNumObs(), 3085);
    }
}