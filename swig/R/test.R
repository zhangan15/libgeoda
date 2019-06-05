library(libgeoda)
a <- test()
gda <- GeoDa("/Users/xun/Box/test_data/natregimes/natregimes.shp")
w <- gda$CreateQueenWeights()
w$num_obs
w$density
