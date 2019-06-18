# build & install libgeoda to ../deps/osx/geoda

echo "create R wrappers..."
swig -c++ -r -I../../deps/libgeoda/include -o libgeoda.cpp ../libgeoda.i

# create R package
rm -rf build
mkdir build
cp -rf pkg-template build/libgeoda
cp libgeoda.R build/libgeoda/R/
cp sf_geoda.R build/libgeoda/R/
cp libgeoda.cpp build/libgeoda/src
cp *.Rmd build/libgeoda/docs
cp *.html build/libgeoda/docs
mkdir build/libgeoda/src/deps
mkdir build/libgeoda/src/deps/libgeoda
cp -rf ../../deps/ build/libgeoda/src/deps/
rm -rf build/libgeoda/src/deps/boost/include
cd build

R CMD BUILD libgeoda
R CMD REMOVE libgeoda
R CMD INSTALL libgeoda_0.1.0.tar.gz

cd ..
