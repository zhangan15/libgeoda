from pygeoda import GeoDa, GeoDaTable, geopandas_to_geoda, geoda_to_geopandas
import geopandas

#gda = GeoDa("/Users/xun/github/libgeoda/data/natregimes.shp")
#gdf = geoda_to_geopandas(gda)

gdf = geopandas.read_file("/Users/xun/github/libgeoda/data/natregimes.shp")
gda = geopandas_to_geoda(gdf)

#ax = gdf.plot()
#ax.set_title("NAT")