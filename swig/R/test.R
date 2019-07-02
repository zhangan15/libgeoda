dyn.load("rgeoda.so")
source("rgeoda.R")
cacheMetaData(1)

source("sf_geoda.R")
#gda <- GeoDa("../../data/natregimes.shp")
#sp_nat <- geoda_to_sp(gda)


library(sf)
nc <- st_read("../../data/natregimes.shp")

gda <- sf_to_geoda(nc)
data <- gda$GetNumericCol("HR60")
queen_w <- gda$CreateContiguityWeights()
lisa <- gda$LISA(queen_w, data)
lags <- lisa$GetLagValues()
lags[1:10]
