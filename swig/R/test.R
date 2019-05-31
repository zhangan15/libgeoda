dyn.load(paste("geoda", .Platform$dynlib.ext, sep=""))
source("geoda.R")
cacheMetaData(1)
# The cacheMetaData(1) will cause R to refresh its object tables. 
# Without it, inheritance of wrapped objects may fail.
gda <- GeoDaProxy("/Users/xun/Box/test_data/natregimes/natregimes.shp")

gda$num_obs