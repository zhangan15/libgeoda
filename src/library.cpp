#include "library.h"

#include <iostream>

#include <wx/wx.h>
#include <ANN/ANN.h>

GeoDaProxy::GeoDaProxy()
: poDS(NULL), poLayer(NULL)
{

}

GeoDaProxy::GeoDaProxy(const char* poDsPath)
: poDS(NULL), poLayer(NULL), numLayers(0), numObs(0)
{
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
    GDALClose(poDS);
}

GeoDaProxy::~GeoDaProxy() {
    //if (poDS) {
    //    GDALClose(poDS);
    //}
}

std::vector<double> GeoDaProxy::GetValues()
{
    std::vector<double> rst;
    rst.push_back(0);
    rst.push_back(1);
    rst.push_back(2);

    return rst;
}

std::string GeoDaProxy::GetName() {
    return "Name";
}

