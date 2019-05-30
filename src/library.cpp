#include "library.h"

#include <iostream>

#include <wx/wx.h>
#include <ANN/ANN.h>
#include <ogrsf_frmts.h>

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