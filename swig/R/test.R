dyn.load("libgeoda.so")
source("libgeoda.R")
cacheMetaData(1)

library(sf)
nc <- st_read(system.file("shape/nc.shp", package="sf"))