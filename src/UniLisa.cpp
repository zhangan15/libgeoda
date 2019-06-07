//
// Created by Xun Li on 2019-06-05.
//

#include "./geoda/GenUtils.h"
#include "./geoda/ShapeOperations/GeodaWeight.h"
#include "UniLisa.h"


UniLisa::~UniLisa() {

}

UniLisa::UniLisa(int num_obs,
        const std::vector<double> &_data,
        const std::vector<bool> &_undefs,
        GeoDaWeight *w)
: AbstractLocalSA(num_obs, w), data(_data), undefs(_undefs),
  CLUSTER_NOT_SIG(0),
  CLUSTER_HIGHHIGH(1),
  CLUSTER_LOWLOW(2),
  CLUSTER_LOWHIGH(3),
  CLUSTER_HIGHLOW(4),
  CLUSTER_UNDEFINED(5),
  CLUSTER_NEIGHBORLESS(6)

{
    lag_vec.resize(num_obs, 0);
    localMoran_vec.resize(num_obs, 0);

    GenUtils::StandardizeData(data, undefs);
    Run();
}

void UniLisa::ComputeLoalSA() {
    for (size_t i=0; i<num_obs; i++) {
        if (undefs[i]) {
            lag_vec[i] = 0;
            localMoran_vec[i] = 0;
            cluster_vec[i] = CLUSTER_UNDEFINED;

            if (weights->GetNbrSize(i) == 0) {
                cluster_vec[i] = CLUSTER_NEIGHBORLESS;
            }

        } else {
            double sp_lag = 0;
            sp_lag = weights->SpatialLag(i, data);
            lag_vec[i] = sp_lag;
            localMoran_vec[i] = data[i] * sp_lag;
            // assign the cluster
            if (data[i] > 0 && sp_lag < 0) cluster_vec[i] = CLUSTER_HIGHLOW;
            else if (data[i] < 0 && sp_lag > 0) cluster_vec[i] = CLUSTER_LOWHIGH;
            else if (data[i] < 0 && sp_lag < 0) cluster_vec[i] = CLUSTER_LOWLOW;
            else cluster_vec[i] = CLUSTER_HIGHHIGH; //data1[i] > 0 && Wdata > 0
        }
    }
}

bool UniLisa::LargerPermLocalSA(int cnt, std::vector<int> &permNeighbors) {
    int validNeighbors = 0;
    double permutedLag = 0;
    int numNeighbors = permNeighbors.size();
    // use permutation to compute the lag
    // compute the lag for binary weights
    for (int cp=0; cp<numNeighbors; cp++) {
        int nb = permNeighbors[cp];
        if (!undefs[nb]) {
            permutedLag += data[nb];
            validNeighbors ++;
        }
    }
    //NOTE: we shouldn't have to row-standardize or
    // multiply by data1[cnt]
    if (validNeighbors > 0 && row_standardize) {
        permutedLag /= validNeighbors;
    }
    const double localMoranPermuted = permutedLag * data[cnt];
    if (localMoranPermuted >= localMoran_vec[cnt]) return true;
    return false;
}

const vector<double> &UniLisa::GetLocalMoranValues() const {
    return localMoran_vec;
}

const vector<double> &UniLisa::GetLagValues() const {
    return lag_vec;
}

std::vector<int> UniLisa::GetClusterIndicators() {
    std::vector<int> clusters(num_obs);
    double cuttoff = GetSignificanceCutoff();
    for (int i=0; i<num_obs; i++) {
        if (sig_local_vec[i] > cuttoff &&
            cluster_vec[i] != CLUSTER_UNDEFINED &&
            cluster_vec[i] != CLUSTER_NEIGHBORLESS)
        {
            clusters[i] = CLUSTER_NOT_SIG;
        } else {
            clusters[i] = cluster_vec[i];
        }
    }
    return clusters;
}

