//
// Created by Xun Li on 2019-06-06.
//

#include <vector>
#include <limits.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <libgeoda.h>
#include <GeodaWeight.h>
#include <UniLisa.h>

using namespace testing;

namespace {

    TEST(CLUSTERING_TEST, SKATER) {
        GeoDa gda("../data/natregimes.shp");
        GeoDaWeight* w = gda.CreateContiguityWeights();
        std::vector<std::string> col_names = {"HR60"};
        std::vector<int> clst = gda.SKATER(5, w, col_names);

        //EXPECT_THAT(gda.by_col<int>(""), ElementsAre(0.0, 1.0, 2.0));
        EXPECT_THAT(w->num_obs, 3085);
    }
}