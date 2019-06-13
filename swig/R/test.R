dyn.load("libgeoda.so")
source("libgeoda.R")
cacheMetaData(1)

gda <- GeoDa("../../data/natregimes.shp")
wkb <- gda$GetGeometryWKB()

library(wkb)
library(rgdal)
nat_sp <- readOGR("../../data/natregimes.shp")

source("sf_geoda.R")
nat <- sp_to_geoda(nat_sp)

wkb <- writeWKB(nat_sp)
test_wkb2(wkb[[1]], length(wkb[[1]]))

n = length(wkb)
m <- sapply(wkb, function(x) return(length(x)))
dat <- VecRaw()
for (i in 1:n) {
    dat$push_back(address(wkb[[i]]))
}
test_wkb5(dat, m)


wkb_vec <- VecVoid()
for (j in seq_along(wkb[[1]])) {
    wkb_vec$push_back( wkb[[1]][[j]] )
}

wkb_vec <- VecChar()
for (j in seq_along(wkb[[1]])) {
    wkb_vec$push_back( rawToChar(wkb[[1]][[j]]) )
}
test_wkb(wkb_vec)


a_val <- VecString()c("a","b")
a<-GeoDaStringColumn("a", a_val)
b_val <- c(1, 2)
b<-GeoDaIntColumn("b", b_val)
c_val <- c(1.1, 2.2)
c<-GeoDaRealColumn("c", c_val)
aa<-ToGeoDaColumn(a)
bb<-ToGeoDaColumn(b)
cc<-ToGeoDaColumn(c)
cols <- VecGeoDaColumn()
cols$push_back(aa)
cols$push_back(bb)
cols$push_back(cc)
test_cols(cols)

#library(sf)
#nc <- st_read(system.file("shape/nc.shp", package="sf"))

