#include "libgeoda.h"

#include <iostream>

#include <ANN/ANN.h>
#include <wx/wx.h>
#include <wx/filename.h>

#include "./geoda/ShapeOperations/PolysToContigWeights.h"
#include "./geoda/ShapeOperations/GalWeight.h"
#include "./geoda/ShapeOperations/GeodaWeight.h"

GeoDa::GeoDa()
: poDS(NULL), poLayer(NULL)
{

}

GeoDa::GeoDa(const char* poDsPath)
: poDS(NULL), poLayer(NULL), numLayers(0), numObs(0)
{
    wxFileName wx_fn(poDsPath);
    GDALAllRegister();
    poDS = (GDALDataset*) GDALOpenEx(poDsPath, GDAL_OF_VECTOR, NULL, NULL, NULL);

    if( poDS ) {
        numLayers = poDS->GetLayerCount();
        if (numLayers > 0) {
            poLayer = poDS->GetLayer(0);
            numObs = poLayer->GetFeatureCount(true);
            std::cout << "count:" << numObs << std::endl;
        }
    } else {
        std::cout << "posDS is NULL" << std::endl;
    }
    //GDALClose(poDS);
}

GeoDa::~GeoDa() {
    if (poDS) {
        GDALClose(poDS);
    }
}

std::vector<double> GeoDa::GetValues()
{
    std::vector<double> rst;
    rst.push_back(0);
    rst.push_back(1);
    rst.push_back(2);

    return rst;
}

std::string GeoDa::GetName() {
    return "Name";
}

GeoDaWeight* GeoDa::CreateQueenWeights(std::string polyid)
{
    double precision_threshold = 0.0;
    bool is_rook = false;

    GalWeight* poW = new GalWeight;
    poW->num_obs = numObs;
    poW->is_symmetric = true;
    poW->symmetry_checked = true;

    poW->gal = PolysToContigWeights(poLayer, !is_rook, precision_threshold);

    poW->GetNbrStats();
    return (GeoDaWeight*)poW;
}

int test() { return 100;}