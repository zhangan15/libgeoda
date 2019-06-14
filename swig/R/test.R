dyn.load("libgeoda.so")
source("libgeoda.R")
cacheMetaData(1)

source("sf_geoda.R")
gda <- GeoDa("../../data/natregimes.shp")

sp_nat <- geoda_to_sp(gda)
