//
// Created by Xun Li on 2019-06-04.
//

#include <limits.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <libgeoda.h>
#include <GeodaWeight.h>

using namespace testing;

namespace {

    TEST(WEIGHTS_TEST, GAL_CREATE) {
        GeoDa gda("../data/natregimes.shp");
        GeoDaWeight* w = gda.CreateContiguityWeights();
        //EXPECT_THAT(gda.by_col<int>(""), ElementsAre(0.0, 1.0, 2.0));
        EXPECT_THAT(w->num_obs, 3085);
    }
}