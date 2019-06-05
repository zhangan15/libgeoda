#ifndef LIBGEODA_LIBRARY_H
#define LIBGEODA_LIBRARY_H

#include <vector>
#include <ogrsf_frmts.h>

class GeoDaWeight;

class GeoDa {
public:
    GeoDa();

    GeoDa(const char* pDsPath);

    virtual ~GeoDa();

    std::vector<double> GetValues();

    std::string GetName();

    GeoDaWeight* CreateQueenWeights(std::string polyid="");

protected:
    GDALDataset *poDS;
    OGRLayer *poLayer;

    int numLayers;
    int numObs;
};

int test();

#endif