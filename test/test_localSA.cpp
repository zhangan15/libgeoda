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

    TEST(LOCALSA_TEST, LISA_UNI) {
        GeoDa gda("../data/natregimes.shp");
        GeoDaWeight* w = gda.CreateQueenWeights();
        std::vector<double> data = gda.GetNumericCol("HR60");
        std::vector<bool> undefs = gda.GetUndefinesCol("HR60");
        UniLisa* lisa = gda.LISA(w, data, undefs);
        std::vector<int> lisa_cluster = lisa->GetClusterIndicators();
        //EXPECT_THAT(gda.by_col<int>(""), ElementsAre(0.0, 1.0, 2.0));
        EXPECT_THAT(w->num_obs, 3085);
    }
}