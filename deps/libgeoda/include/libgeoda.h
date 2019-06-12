#ifndef LIBGEODA_LIBRARY_H
#define LIBGEODA_LIBRARY_H

#include <list>
#include <vector>
#include <map>
#include <ogrsf_frmts.h>

class UniLisa;
class GeoDaWeight;

class GeoDaColumn {
public:
    enum FieldType { integer_type, string_type, real_type };
    std::string name;
    FieldType field_type;
    int field_length;
    int field_decimals;

    GeoDaColumn(const std::string& nm, FieldType ft, int flen, int fdec)
    : name(nm), field_type(ft), field_length(flen), field_decimals(fdec) {}
    virtual ~GeoDaColumn() {}
    virtual void SetData(const std::vector<long long>& vals) { }
};

class GeoDaIntColumn : public GeoDaColumn {
public:
    std::vector<long long> data;

    virtual std::vector<long long>& GetData() { return data;}
    virtual void SetData(const std::vector<long long>& vals) { data = vals; }

    GeoDaIntColumn(const std::string& nm, const std::vector<long long>& vals)
    : GeoDaColumn(nm, integer_type, 20, 0), data(vals) {}
};


class GeoDaStringColumn : public GeoDaColumn {
public:
    std::vector<std::string> data;

    virtual std::vector<std::string>& GetData() { return data;}
    virtual void SetData(const std::vector<std::string>& vals) { data = vals; }

    GeoDaStringColumn(const std::string& nm, const std::vector<std::string>& vals)
            : GeoDaColumn(nm, string_type, 254, 0), data(vals) {}
};

class GeoDaRealColumn : public GeoDaColumn {
public:
    std::vector<double> data;

    virtual std::vector<double>& GetData() { return data;}
    virtual void SetData(const std::vector<double>& vals) { data = vals; }

    GeoDaRealColumn(const std::string& nm, const std::vector<double>& vals)
            : GeoDaColumn(nm, real_type, 35, 15), data(vals) {}
};

class GeoDa {
public:
    enum MapType { point_type, polygon_type, line_type };

    GeoDa(const std::string& layer_name, const std::string& map_type,
            int num_features,
            const std::vector<GeoDaColumn*>& cols,
            const std::vector<std::vector<char>>& wkbs,
            const char* pszProj4);

    GeoDa(const char* pDsPath, const char* layer_name=NULL);

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

    OGRGeometry* CreateOGRGeomFromWkb(const std::vector<char>& wkb);

protected:
    static const std::string DT_STRING;
    static const std::string DT_INTEGER;
    static const std::string DT_NUMERIC;

    GDALDataset *poDS;
    OGRLayer *poLayer;
    OGRSpatialReference *poSpatialRef;
    MapType mapType;

    int numLayers;
    int numObs;
    int numCols;

    std::vector<std::string> fieldNames;
    std::vector<std::string> fieldTypes;
    std::map<std::string, unsigned int> fieldNameIdx;

};

int test();
void test_wkb1(int n, unsigned char* wkb);
void test_wkb2(unsigned char* wkb, int n);
void test_wkb3(int n, std::vector<int>& m, unsigned char** wkbs);
void test_wkb4(int n, std::vector<int>& m, std::vector<unsigned char*>& wkbs);
void test_wkb5(std::vector<unsigned char*>& wkbs, std::vector<int> m);
void test_wkb(const std::vector<void*>& wkbs);
void test_cols(std::vector<GeoDaColumn*> cols);

GeoDaColumn* ToGeoDaColumn(GeoDaStringColumn* col);
GeoDaColumn* ToGeoDaColumn(GeoDaIntColumn* col);
GeoDaColumn* ToGeoDaColumn(GeoDaRealColumn* col);
#endif