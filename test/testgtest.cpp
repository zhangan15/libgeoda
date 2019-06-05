//
// Created by Xun Li on 5/30/19.
//

#include <limits.h>
#include <libgeoda.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

namespace {

    TEST(IOTEST, NAT_FEAT_COUNT) {
        GeoDa gda("../data/natregimes.shp");
        EXPECT_THAT(gda.GetValues(), ElementsAre(0.0, 1.0, 2.0));
    }
}

