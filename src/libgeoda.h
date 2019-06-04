#ifndef LIBGEODA_LIBRARY_H
#define LIBGEODA_LIBRARY_H

#include <vector>
#include <ogrsf_frmts.h>


class GeoDaProxy {
public:
    GeoDaProxy();

    GeoDaProxy(const char* pDsPath);

    virtual ~GeoDaProxy();

    std::vector<double> GetValues();

    std::string GetName();

protected:
    GDALDataset *poDS;
    OGRLayer *poLayer;

    int numLayers;
    int numObs;
};

#endif