#include "library.h"

#include <iostream>

#include <wx/wx.h>
#include <ANN/ANN.h>
#include <ogrsf_frmts.h>

int CountFeatures(const char *ds_path)
{
    GDALAllRegister();
    GDALDataset *poDS;
    poDS = (GDALDataset*) GDALOpenEx(ds_path, GDAL_OF_VECTOR, NULL, NULL, NULL);

    if( poDS == NULL ) {
        std::cout << "posDS is NULL" << std::endl;
        return 0;
    }
    OGRLayer *poLayer = NULL;
    int n_layers = poDS->GetLayerCount();
    if (n_layers > 0) {
        poLayer = poDS->GetLayer(0);
        int n = poLayer->GetFeatureCount(true);
        std::cout << "count:" << n << std::endl;
        return n;
    }
    GDALClose(poDS);
    return 0;
}

std::vector<const char*> GetLayerNames(const char *ds_path)
{
    std::vector<const char*> results;
    GDALAllRegister();
    GDALDataset *poDS;
    poDS = (GDALDataset*) GDALOpenEx(ds_path, GDAL_OF_VECTOR, NULL, NULL, NULL);

    if( poDS == NULL ) {
        return results;
    }
    OGRLayer *poLayer = NULL;
    int n_layers = poDS->GetLayerCount();
    for (size_t i=0; i<n_layers; i++)  {
        poLayer = poDS->GetLayer(i);
        const char* name_ref = poLayer->GetName();
        char* name = new char[strlen(name_ref)];
        name = strcpy(name, name_ref);
        results.push_back(name);
    }
    GDALClose(poDS);
    return results;
}

void hello() {
    wxString test = "hello, wrold";
    std::cout << test.c_str() << std::endl;
}

int Factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }

    return result;
}