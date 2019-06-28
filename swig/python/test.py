from pygeoda import GeoDa, GeoDaTable, geopandas_to_geoda, geoda_to_geopandas
import geopandas
import ctypes
import matplotlib

gda = GeoDa("../../data/natregimes.shp")

gdf = geoda_to_geopandas(gda)

ax = gdf.plot()

ax.set_title("NAT")
"""
gdf = geopandas.read_file("../../data/natregimes.shp")

gda = geopandas_to_geoda(gdf)

w = gda.CreateContiguityWeights()
w.density
w.sparsity
"""