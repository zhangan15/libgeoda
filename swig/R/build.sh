# build & install libgeoda to ../deps/osx/geoda

echo "create R wrappers..."
swig -c++ -r -I../../deps/libgeoda/include -o libgeoda.cpp ../libgeoda.i

# create R package
rm -rf build
mkdir build
cp -rf pkg-template build/libgeoda

mkdir build/libgeoda/R
mkdir build/libgeoda/docs
cp -rf ../../deps/ build/libgeoda/src/

cp libgeoda.R build/libgeoda/R/
cp sf_geoda.R build/libgeoda/R/
cp libgeoda.cpp build/libgeoda/src
cp *.Rmd build/libgeoda/docs
cp *.html build/libgeoda/docs
cd build

R CMD build libgeoda
R CMD REMOVE libgeoda
R CMD INSTALL libgeoda_0.1.0.tar.gz

cd ..
