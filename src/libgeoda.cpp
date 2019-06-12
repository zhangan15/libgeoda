#include "libgeoda.h"

#include <iostream>
#include <string.h>

#include <ANN/ANN.h>
#include <wx/wx.h>
#include <wx/filename.h>
#include <boost/algorithm/string.hpp>

#include "./geoda/ShapeOperations/PolysToContigWeights.h"
#include "./geoda/ShapeOperations/GalWeight.h"
#include "./geoda/ShapeOperations/GwtWeight.h"
#include "./geoda/ShapeOperations/GeodaWeight.h"
#include "./geoda/Algorithms/redcap.h"
#include "./geoda/Algorithms/cluster.h"
#include "./geoda/GenUtils.h"

#include "UniLisa.h"

const std::string GeoDa::DT_STRING = "string";
const std::string GeoDa::DT_INTEGER= "integer";
const std::string GeoDa::DT_NUMERIC = "numeric";

GeoDa::GeoDa(const std::string &layer_name,
             const std::string& map_type,
        int num_features,
        const std::vector<GeoDaColumn *> &cols,
        const std::vector<unsigned char*>& wkbs,
        const std::vector<int>& wkb_bytes_len,
        const char* pszProj4)
: poDS(NULL), poLayer(NULL), numObs(num_features), numCols(cols.size())
{
    std::string ds_name = "/vsimem/" + layer_name + ".shp";
    poSpatialRef = new OGRSpatialReference();
    poSpatialRef->importFromProj4(pszProj4);

    OGRwkbGeometryType eGType = wkbPolygon;
    if (map_type == "map_polygons") {
        mapType = polygon_type;
    } else if (map_type == "map_points") {
        eGType = wkbPoint;
        mapType = point_type;
    } else if (map_type == "map_lines") {
        eGType = wkbLineString;
        mapType = line_type;
    } else {
        std::cout << "map type is not supported." << std::endl;
    }

    GDALAllRegister();
    GDALDriver *poDriver = NULL;
    poDriver = GetGDALDriverManager()->GetDriverByName("ESRI Shapefile");

    if (poDriver) {
        poDS = poDriver->Create( ds_name.c_str(), 0,0,0, GDT_Unknown, NULL);
        if( poDS ) {
            std::cout << "create a in-memory data source" << std::endl;
            poLayer = poDS->CreateLayer(layer_name.c_str(), poSpatialRef, eGType);
            // create fields
            for (size_t i=0; i<cols.size(); ++i) {
                GeoDaColumn* col = cols[i];

                OGRFieldType ft = OFTString;
                if (col->field_type == GeoDaColumn::integer_type) ft = OFTInteger64;
                else if (col->field_type == GeoDaColumn::real_type) ft = OFTReal;
                OGRFieldDefn oField(col->name.c_str(), ft);
                oField.SetWidth(col->field_length);
                if (col->field_type == GeoDaColumn::real_type) oField.SetPrecision(col->field_decimals);

                if( poLayer->CreateField( &oField ) != OGRERR_NONE ) {
                    std::cout << "Creating field " << col->name << " failed." << std::endl;
                }
                fieldNames.push_back(col->name);
                fieldNameIdx[col->name] = i;
                if (ft == OFTInteger64 || ft == OFTInteger) {
                    fieldTypes.push_back(DT_INTEGER);
                } else if (ft == OFTReal) {
                    fieldTypes.push_back(DT_NUMERIC);
                } else {
                    fieldTypes.push_back(DT_STRING);
                }
            }
            // create features
            for (size_t i=0; i<num_features; ++i) {
                OGRFeature *poFeature;
                poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
                for (size_t j=0; j<cols.size(); ++j) {
                    GeoDaColumn *col = cols[j];
                    if (col->field_type == GeoDaColumn::integer_type) {
                        GeoDaIntColumn *int_col = (GeoDaIntColumn*)col;
                        std::vector<long long>& data = int_col->GetData();
                        poFeature->SetField(col->name.c_str(), data[i]);

                    } else if (col->field_type == GeoDaColumn::real_type) {
                        GeoDaRealColumn *real_col = (GeoDaRealColumn*)col;
                        std::vector<double>& data = real_col->GetData();
                        poFeature->SetField(col->name.c_str(), data[i]);

                    } else if (col->field_type == GeoDaColumn::string_type) {
                        GeoDaStringColumn *_col = (GeoDaStringColumn*)col;
                        std::vector<std::string>& data = _col->GetData();
                        poFeature->SetField(col->name.c_str(), data[i].c_str());
                    }
                }

                OGRGeometry* geom = CreateOGRGeomFromWkb(wkbs[i], wkb_bytes_len[i]);
                poFeature->SetGeometry(geom);

                if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE ) {
                    std::cout << "Failed to create feature in shapefile.\n" << std::endl;
                }
                OGRFeature::DestroyFeature( poFeature );
            }
        }
    }
}

GeoDa::GeoDa(const char* poDsPath, const char* layer_name)
: poDS(NULL), poLayer(NULL), numLayers(0), numObs(0)
{
    wxFileName wx_fn(poDsPath);
    GDALAllRegister();
    poDS = (GDALDataset*) GDALOpenEx(poDsPath, GDAL_OF_VECTOR, NULL, NULL, NULL);

    if( poDS ) {
        numLayers = poDS->GetLayerCount();
        if (numLayers > 0) {
            int lyr_idx = 0;
            if (layer_name != NULL) {
                for (size_t i=0; i<numLayers;++i) {
                    if (strcmp(layer_name, poDS->GetLayer(i)->GetName()) == 0) {
                        lyr_idx = i;
                        break;
                    }
                }
            }
            poLayer = poDS->GetLayer(lyr_idx);
            numObs = poLayer->GetFeatureCount(true);
            poSpatialRef = poLayer->GetSpatialRef();
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
            fieldNameIdx[fieldName] = col_idx;
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


OGRGeometry* GeoDa::CreateOGRGeomFromWkb(unsigned char* wkb, int n)
{
    OGRGeometry* geom = 0;
    OGRGeometryFactory::createFromWkb(wkb, 0, &geom, n);
    //char* json = geom->exportToJson();
    //std::cout << geom->WkbSize() << "json:" << json << std::endl;
    return geom;
}

std::string GeoDa::GetName() {
    return "Name";
}

GeoDaWeight* GeoDa::CreateQueenWeights(std::string polyid, int order, bool include_lower_order)
{
    double precision_threshold = 0.0;
    bool is_rook = false;

    GalWeight* poW = new GalWeight;
    poW->num_obs = numObs;
    poW->is_symmetric = true;
    poW->symmetry_checked = true;

    poW->gal = PolysToContigWeights(poLayer, !is_rook, precision_threshold);

    if (order > 1) {
        Gda::MakeHigherOrdContiguity(order, numObs, poW->gal, include_lower_order);
    }
    poW->GetNbrStats();
    return (GeoDaWeight*)poW;
}

std::vector<bool> GeoDa::GetUndefinesCol(std::string col_name) {
    std::vector<bool> rst;
    if (fieldNameIdx.find(col_name) != fieldNameIdx.end()) {
        int iField = fieldNameIdx[col_name];
        OGRFeature *feature = NULL;
        poLayer->ResetReading();
        while ((feature = poLayer->GetNextFeature()) != NULL) {
            bool val = feature->IsFieldNull(iField);
            rst.push_back(val);
        }
    }
    return rst;
}

std::vector<double> GeoDa::GetNumericCol(std::string col_name)
{
    std::vector<double> rst;
    OGRFeature* feature = NULL;
    poLayer->ResetReading();
    while ((feature = poLayer->GetNextFeature()) != NULL) {
        double val = feature->GetFieldAsDouble(col_name.c_str());
        rst.push_back(val);
    }
    return rst;
}

std::vector<long long> GeoDa::GeIntegerCol(std::string col_name) {
    std::vector<long long> rst;
    OGRFeature* feature = NULL;
    poLayer->ResetReading();
    while ((feature = poLayer->GetNextFeature()) != NULL) {
        long long val = feature->GetFieldAsInteger64(col_name.c_str());
        rst.push_back(val);
    }
    return rst;
}

std::vector<std::string> GeoDa::GetStringCol(std::string col_name) {
    std::vector<std::string> rst;
    OGRFeature* feature = NULL;
    poLayer->ResetReading();
    while ((feature = poLayer->GetNextFeature()) != NULL) {
        std::string val = feature->GetFieldAsString(col_name.c_str());
        rst.push_back(val);
    }
    return rst;
}

std::vector<std::string> GeoDa::GetFieldNames() {
    return fieldNames;
}

std::vector<std::string> GeoDa::GetFieldTypes() {
    return fieldTypes;
}

int GeoDa::GetNumObs() const {
    return numObs;
}

int GeoDa::GetNumCols() const {
    return numCols;
}

UniLisa* GeoDa::LISA(GeoDaWeight *w, const std::vector<double> &data,
        const std::vector<bool> &undefs)
{
    std::vector<bool> copy_undefs = undefs;
    if (copy_undefs.empty()) {
        copy_undefs.resize(numObs, false);
    }
    UniLisa* lisa = new UniLisa(numObs, data, copy_undefs, w);
    return lisa;
}

const std::vector<int> GeoDa::SKATER(unsigned int k, GeoDaWeight *w,
        std::vector<std::string> col_names,
        const std::string& distance_method,
        const std::string& control_varible,
        double control_threshold)
{
    std::vector<int> rst;
    // check input column names
    int n_cols = col_names.size();
    std::vector<int> col_ids;
    for (size_t i=0; i<n_cols; ++i) {
        std::string col_name = col_names[i];
        if (fieldNameIdx.find(col_name) == fieldNameIdx.end()) {
            std::cout << "col name not found" << std::endl;
            return rst;
        }
        int col_idx = fieldNameIdx[col_name];
        if (fieldTypes[col_idx] == "string") {
            std::cout << "col has to be numeric column" << std::endl;
            return rst;
        }
        col_ids.push_back(col_idx);
    }
    std::vector<std::vector<double> > data(n_cols);
    OGRFeature *feature = NULL;
    poLayer->ResetReading();
    while ((feature = poLayer->GetNextFeature()) != NULL) {
        for (size_t i=0; i<n_cols; ++i) {
            int iField = col_ids[i];
            double val = feature->GetFieldAsDouble(iField);
            data[i].push_back(val);
        }
    }
    rst = SKATER(k, w, data, distance_method, control_varible, control_threshold);
    return rst;
}

const std::vector<int>
GeoDa::SKATER(unsigned int k, GeoDaWeight *w,
        std::vector<std::vector<double> >& data,
        const string &distance_method,
        const string &control_varible,
        double control_threshold)
{
    std::vector<int> rst;
    int n_cols = data.size();
    double** matrix = new double*[numObs];
    int** mask = new int*[numObs];
    for (size_t i=0; i<numObs; ++i) {
        matrix[i] = new double[n_cols];
        mask[i] = new int[n_cols];
        for (size_t j=0; j<n_cols; ++j) mask[i][j] = 1.0;
    }
    for (size_t i=0; i<n_cols; ++i) {
        std::vector<double>& vals = data[i];
        GenUtils::StandardizeData(vals);
        for (size_t r=0; r<numObs; ++r) {
            matrix[r][i] = vals[r];
        }
    }
    char dist = 'e';
    if (boost::iequals(distance_method, "manhattan")) dist = 'b';
    int transpose = 0; // row wise
    double* weight = new double[n_cols];
    for (size_t i=0; i<n_cols; ++i) weight[i] = 1.0;

    double** ragged_distances = distancematrix(numObs, n_cols, matrix,  mask, weight, dist, transpose);
    double** distances = fullRaggedMatrix(ragged_distances, numObs, numObs);
    for (size_t i = 1; i < numObs; i++) free(ragged_distances[i]);
    free(ragged_distances);

    rst = SKATER(k, w, numObs, n_cols, distances, matrix);

    // free memory
    for (size_t i = 1; i < numObs; i++) free(distances[i]);
    free(distances);
    delete[] weight;
    //delete[] bound_vals;
    for (size_t i=0; i<numObs; ++i) delete[] matrix[i];
    delete[] matrix;

    return rst;
}

const std::vector<int> GeoDa::SKATER(unsigned int k, GeoDaWeight *w, int rows, int columns,
        double** distances, double **input_data,
        double* bound_vals, double min_bound)
{
    std::vector<int> rst;
    std::vector<bool> undefs; // not used
    GalWeight* gal_w = 0;
    if (w->weight_type == GeoDaWeight::gal_type) {
        gal_w = dynamic_cast<GalWeight*>(w);
    } else {
        GwtWeight* gwt_w = dynamic_cast<GwtWeight*>(w);

    }
    if (gal_w == 0) return rst;
    SpanningTreeClustering::AbstractClusterFactory* redcap = 0;
    redcap = new SpanningTreeClustering::FirstOrderSLKRedCap(rows, columns,
            distances, input_data, undefs, gal_w->gal, bound_vals, min_bound);
    if (redcap == 0) return rst;

    redcap->Partitioning(k);
    vector<vector<int> > cluster_ids = redcap->GetRegions();
    int ncluster = cluster_ids.size();
    rst.resize(numObs,0);
    std::sort(cluster_ids.begin(), cluster_ids.end(), GenUtils::less_vectors);
    for (int i=0; i < ncluster; i++) {
        int c = i + 1;
        for (int j=0; j<cluster_ids[i].size(); j++) {
            int idx = cluster_ids[i][j];
            rst[idx] = c;
        }
    }
    // in case c == 0
    for (int i=0; i<rst.size(); i++) {
        if (rst[i] == 0) {
            rst[i] = ncluster + 1;
        }
    }
    delete redcap;
    return rst;
}

double** GeoDa::fullRaggedMatrix(double** matrix, int n, int k, bool isSqrt) {
    double** copy = new double*[k];

    for (int i = 0; i < k; i++) {
        copy[i] = new double[n];
        for (int j = 0; j < n; j++)
            copy[i][j] = 0;
    }

    for (int i = 1; i < k; i++) {
        for (int j = 0; j < i; j++) {
            if (isSqrt) copy[i][j] = sqrt(matrix[i][j]);
            copy[i][j] = matrix[i][j];
            copy[j][i] = copy[i][j];
        }
    }

    return copy;
}


int test() { return 100;}

void test_wkb1(int n, unsigned char* wkb) {
    OGRPolygon* geom = new OGRPolygon();
    geom->importFromWkb(wkb, n);
    delete geom;
}
void test_wkb2(unsigned char* wkb, int n) {
    OGRPolygon* geom = new OGRPolygon();
    geom->importFromWkb(wkb, n);
    delete geom;
}

void test_wkb(const std::vector<void*>& wkbs) {

    //for (size_t i=0; i<wkbs.size(); ++i) {
        int len = wkbs.size();
        unsigned char* val = new unsigned char[len];
        for (size_t j=0; j<len; ++j) {
            val[j] = *(unsigned char*)wkbs[j];
            std::cout << val[j] <<std::endl;
        }

        //OGRGeometryFactory::createFromWkb(val, 0, &geom, len);
        OGRPolygon* geom = new OGRPolygon();
        geom->importFromWkb(val, len);
        char* json = geom->exportToJson();
        std::cout << geom->WkbSize() << "json:" << json << std::endl;
        delete[] val;
        delete geom;
}

void test_cols(std::vector<GeoDaColumn*> cols)
{
    for (size_t i=0; i<cols.size(); ++i) {
        std::cout << cols[i]->name << std::endl;
    }
}


GeoDaColumn* ToGeoDaColumn(GeoDaStringColumn* col)
{
    return (GeoDaColumn*) col;
}

GeoDaColumn *ToGeoDaColumn(GeoDaIntColumn *col) {
    return (GeoDaColumn*) col;
}

GeoDaColumn *ToGeoDaColumn(GeoDaRealColumn *col) {
    return (GeoDaColumn*) col;
}

void test_wkb3(int n, std::vector<int> &m, unsigned char **wkbs) {

}

void test_wkb4(int n, std::vector<int> &m, std::vector<unsigned char *> &wkbs) {
    for (size_t i=0; i<n; ++i) {
        unsigned char* wkb = wkbs[i];
        for (size_t j=0; j<m[i]; ++j) {
            OGRPolygon* geom = new OGRPolygon();
            geom->importFromWkb(wkb, m[i]);
            delete geom;
        }
    }
}

void test_wkb5(std::vector<unsigned char *> &wkbs, std::vector<int> m) {
    for (size_t i=0; i<m.size(); ++i) {
        unsigned char* wkb = wkbs[i];
        for (size_t j=0; j<m[i]; ++j) {
            OGRPolygon* geom = new OGRPolygon();
            geom->importFromWkb(wkb, m[i]);
            delete geom;
        }
    }
}
