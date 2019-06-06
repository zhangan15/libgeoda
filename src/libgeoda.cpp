#include "libgeoda.h"

#include <iostream>

#include <ANN/ANN.h>
#include <wx/wx.h>
#include <wx/filename.h>

#include "./geoda/ShapeOperations/PolysToContigWeights.h"
#include "./geoda/ShapeOperations/GalWeight.h"
#include "./geoda/ShapeOperations/GeodaWeight.h"

const std::string GeoDa::DT_STRING = "string";
const std::string GeoDa::DT_INTEGER= "integer";
const std::string GeoDa::DT_NUMERIC = "numeric";

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
            std::cout << "feature count:" << numObs << std::endl;
        }
        // read field info
        OGRFeatureDefn* featureDefn = poLayer->GetLayerDefn();
        numCols = featureDefn->GetFieldCount();
        std::cout << "field names count:" << numCols << std::endl;

        for (size_t col_idx=0; col_idx < numCols; col_idx++) {
            OGRFieldDefn *fieldDefn = featureDefn->GetFieldDefn(col_idx);
            std::string fieldName = fieldDefn->GetNameRef();
            fieldNames.push_back(fieldName);

            OGRFieldType fieldType = fieldDefn->GetType();
            if (fieldType == OFTInteger64 || fieldType == OFTInteger) {
                fieldTypes.push_back(DT_INTEGER);
            } else if (fieldType == OFTReal) {
                fieldTypes.push_back(DT_NUMERIC);
            } else {
                fieldTypes.push_back(DT_STRING);
            }
            //std::cout << "field name: " << fieldName << " field type: " << fieldType << std::endl;
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

std::vector<double> GeoDa::GetNumericCol(std::string col_name)
{
    std::vector<double> rst;

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

    //if (order > 1)
    //    Gda::MakeHigherOrdContiguity(order, n_obs, gal, include_lower_order);
    poW->GetNbrStats();
    return (GeoDaWeight*)poW;
}



std::vector<int64_t> GeoDa::GeIntegerCol(std::string col_name) {
    return std::vector<int64_t>();
}

std::vector<std::string> GeoDa::GetStringCol(std::string col_name) {
    return std::vector<std::string>();
}

const std::vector<std::string> &GeoDa::getFieldNames() const {
    return fieldNames;
}

const std::vector<std::string> &GeoDa::getFieldTypes() const {
    return fieldTypes;
}

int GeoDa::getNumObs() const {
    return numObs;
}

int GeoDa::getNumCols() const {
    return numCols;
}

void LISA()
{
    //LisaCoordinator* lc = new LisaCoordinator(w_path, num_obs, var_1, var_2, lisa_type, numPermutations);
}

int test() { return 100;}