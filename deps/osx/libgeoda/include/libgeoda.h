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

    // Layer functions
    int getNumObs() const;
    int getNumCols() const;
    const std::vector<std::string> &getFieldTypes() const;
    const std::vector<std::string> &getFieldNames() const;

    // Data functions
    std::vector<double> GetNumericCol(std::string col_name);
    std::vector<int64_t> GeIntegerCol(std::string col_name);
    std::vector<std::string> GetStringCol(std::string col_name);

    std::string GetName();

    // Weights functions
    GeoDaWeight* CreateQueenWeights(std::string polyid="");

    // LISA functions


protected:
    static const std::string DT_STRING;
    static const std::string DT_INTEGER;
    static const std::string DT_NUMERIC;

    GDALDataset *poDS;
    OGRLayer *poLayer;

    int numLayers;
    int numObs;
    int numCols;

    std::vector<std::string> fieldNames;
    std::vector<std::string> fieldTypes;
};

int test();

#endif