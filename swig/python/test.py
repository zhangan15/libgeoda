from pygeoda import GeoDa, GeoDaTable
import geopandas

#gda = GeoDa("../../data/natregimes.shp")

gdf = geopandas.read_file("../../data/natregimes.shp")

gdf.crs

geoms = gdf.geometry
gdf.geom_type

n = len(geoms)

geom = geoms[0] # shapely object

geom.to_wkb() # return 'bytes'


n_rows = len(gdf)
n_cols = gdf.columns.size
col_nms = gdf.columns

# Table
gda_tbl = GeoDaTable()

for i in range(n_cols):
    col_nm = col_nms[i]
    if col_nm == 'geometry':
        continue
    col_type = gdf[col_nm].dtype
    vals = gdf[col_nm].to_list()
    if col_type == 'float64' or col_type == 'float':
        gda_tbl.AddRealColumn(col_nm, vals)
    elif col_type == 'int64' or col_type == 'int':
        gda_tbl.AddIntColumn(col_nm, vals)
    else:
        gda_tbl.AddStringColumn(col_nm, vals)

# Geoms
wkb_bytes = bytes()
wkb_size = []
for i in range(n_rows):
    wkb = geoms[i].to_wkb()
    wkb_bytes += wkb
    wkb_size.append(len(wkb))

map_type = "map_polygons"
file_name = "virtual.shp"
prj = ""
gda = GeoDa(n_rows, file_name, map_type, gda_tbl, wkb_bytes, wkb_size, prj)




