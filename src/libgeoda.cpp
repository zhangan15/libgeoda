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
#include "./geoda/ShapeOperations/VoronoiUtils.h"
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
        GeoDaTable* table,
        unsigned char* wkbs,
        const std::vector<int>& wkb_bytes_len,
        const std::string& pszProj4)
: poDS(NULL), poLayer(NULL), numObs(num_features), numCols(table->GetNumCols())
{
    std::string ds_name = "/tmp/" + layer_name + ".shp";
    poSpatialRef = new OGRSpatialReference();
    poSpatialRef->importFromProj4(pszProj4.c_str());

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
            std::cout << "create a in-memory data source:" << ds_name << std::endl;
            poLayer = poDS->CreateLayer(layer_name.c_str(), poSpatialRef, eGType);
            // create fields
            for (size_t i=0; i<numCols; ++i) {
                GeoDaColumn *col = table->GetColumn(i);
                std::cout << "field: " << col->name << col->name.c_str() << std::endl;
            }
            for (size_t i=0; i<numCols; ++i) {
                GeoDaColumn* col = table->GetColumn(i);

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
            unsigned long long wkb_offset = 0;
            for (size_t i=0; i<num_features; ++i) {
                OGRFeature *poFeature;
                poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
                for (size_t j=0; j<numCols; ++j) {
                    GeoDaColumn *col = table->GetColumn(j);
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

                OGRGeometry* geom = CreateOGRGeomFromWkb(wkbs + wkb_offset, wkb_bytes_len[i]);
                if (geom) poFeature->SetGeometry(geom);
                wkb_offset += wkb_bytes_len[i];

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
            OGRwkbGeometryType geom_type = poLayer->GetGeomType();
            if (geom_type == wkbPolygon || geom_type == wkbMultiPolygon || geom_type == wkbPolygon25D || geom_type ==
            wkbMultiPolygon25D)
                mapType = polygon_type;
            else if (geom_type == wkbPoint || geom_type == wkbMultiPoint || geom_type == wkbPoint25D || geom_type ==
            wkbMultiPoint25D)
                mapType = point_type;
            else if (geom_type == wkbLineString || geom_type == wkbMultiLineString)
                mapType = line_type;
            else {
                std::cout << "map type is not supported" << std::endl;
                return;
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

GeoDaWeight* GeoDa::CreateContiguityWeights(bool is_queen, std::string polyid, int order, bool include_lower_order, double precision_threshold)
{
    GalWeight* poW = new GalWeight;
    poW->num_obs = numObs;
    poW->is_symmetric = true;
    poW->symmetry_checked = true;

    if (mapType == point_type) {
        std::vector<std::set<int> > nbr_map;
        const std::vector<OGRPoint*>& centroids = GetCentroids();
        std::vector<double> x(numObs), y(numObs);
        for (size_t i=0; i<numObs; ++i) {
            x[i] = centroids[i]->getX();
            y[i] = centroids[i]->getX();
        }
        Gda::VoronoiUtils::PointsToContiguity(x, y, is_queen, nbr_map);
        poW->gal = Gda::VoronoiUtils::NeighborMapToGal(nbr_map);

    } else if (mapType == polygon_type) {
        poW->gal = PolysToContigWeights(poLayer, is_queen, precision_threshold);
        if (order > 1) {
            Gda::MakeHigherOrdContiguity(order, numObs, poW->gal, include_lower_order);
        }

    } else {
        // not supported yet
        delete poW;
        return 0;
    }
    poW->GetNbrStats();
    return (GeoDaWeight*)poW;
}

GeoDaWeight* GeoDa::CreateDistanceWeights(double dist_thres, double power, bool is_inverse)
{
    //SpatialIndAlgs::thresh_build();
    return 0;
}

const std::vector<OGRPoint*>& GeoDa::GetCentroids()
{
    if (centroids.empty()) {
        OGRFeature *feature = NULL;
        poLayer->ResetReading();
        while ((feature = poLayer->GetNextFeature()) != NULL) {
            OGRGeometry *geometry = feature->GetGeometryRef();
            if (geometry) {
                OGRPoint *poPoint = new OGRPoint;
                geometry->Centroid(poPoint);
                centroids.push_back(poPoint);
            }
        }
    }
    return centroids;
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

// i starts from 1
std::vector<std::vector<unsigned char> >  GeoDa::GetGeometryWKB() {
    std::vector<std::vector<unsigned char> > rst(numObs);
    OGRFeature *feature = NULL;
    poLayer->ResetReading();
    int row = 0;
    while ((feature = poLayer->GetNextFeature()) != NULL) {
        OGRGeometry* geom = feature->GetGeometryRef();
        int sz = geom->WkbSize();
        rst[row].resize(sz);
        unsigned char *data = (unsigned char*) malloc(sz);
        OGRErr err = geom->exportToWkb(wkbNDR, data, wkbVariantIso);
        //char *text = NULL;
        //geom->exportToWkt(&text);
        for (size_t i=0; i<sz; ++i) {
            rst[row][i] = data[i];
        }
        free(data);
        row++;
    }
    return rst;
}

std::vector<std::string>  GeoDa::GetGeometryWKT() {
    std::vector<std::string> rst(numObs);
    OGRFeature *feature = NULL;
    poLayer->ResetReading();
    int row = 0;
    while ((feature = poLayer->GetNextFeature()) != NULL) {
        OGRGeometry* geom = feature->GetGeometryRef();
        char *text = NULL;
        geom->exportToWkt(&text);
        rst[row] = text;
        row++;
    }
    return rst;
}

GeoDa::MapType GeoDa::GetMapType() const {
    return mapType;
}

GeoDaColumn* ToGeoDaColumn(GeoDaStringColumn* col)
{
    return dynamic_cast<GeoDaColumn*>(col);
}

GeoDaColumn *ToGeoDaColumn(GeoDaIntColumn *col) {
    return dynamic_cast<GeoDaColumn*>(col);
}

GeoDaColumn *ToGeoDaColumn(GeoDaRealColumn *col) {
    return dynamic_cast<GeoDaColumn*>(col);
}


int test() { return 100;}


