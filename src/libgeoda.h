#ifndef LIBGEODA_LIBRARY_H
#define LIBGEODA_LIBRARY_H

#include <vector>
#include <map>
#include <ogrsf_frmts.h>

class UniLisa;
class GeoDaWeight;

class GeoDa {
public:
    //
    GeoDa(const std::string& layer_name);
    GeoDa(const char* pDsPath);

    // create GeoDa instance from table+geometries directly,
    // e.g.
    // (R) GeoDa(sf)
    // (Python) GeoDa(geopandas)
    // GeoDa(Table tbl, Geometry geoms);

    virtual ~GeoDa();

    // Layer functions
    int GetNumObs() const;
    int GetNumCols() const;
    std::vector<std::string> GetFieldTypes();
    std::vector<std::string> GetFieldNames();

    // Data functions
    std::vector<double> GetNumericCol(std::string col_name);
    std::vector<long long> GeIntegerCol(std::string col_name);
    std::vector<std::string> GetStringCol(std::string col_name);
    std::vector<bool> GetUndefinesCol(std::string col_name);

    std::string GetName();

    // Weights functions
    GeoDaWeight* CreateQueenWeights(std::string polyid="", int order=1,
            bool include_lower_order = false);

    // LocalSA functions
    UniLisa* LISA(GeoDaWeight* w, const std::vector<double>& data,
            const std::vector<bool>& undefs = std::vector<bool>());

    // Clustering
    const std::vector<int> SKATER(unsigned int k, GeoDaWeight* w,
            std::vector<std::string> col_names,
            const std::string& distance_method="euclidean",
            const std::string& control_varible="",
            double control_threshold=0);
    const std::vector<int> SKATER(unsigned int k, GeoDaWeight* w,
            std::vector<std::vector<double> >& data,
            const std::string& distance_method="euclidean",
            const std::string& control_varible="",
            double control_threshold=0);

private:
    const std::vector<int> SKATER(unsigned int k, GeoDaWeight* w,
            int n_rows, int n_cols, double** distances, double** data,
            double* bound_vals=0, double min_bound=0);

    double** fullRaggedMatrix(double** matrix, int n, int k, bool isSqrt=false) ;

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
    std::map<std::string, unsigned int> fieldNameIdx;

};

int test();


#endif