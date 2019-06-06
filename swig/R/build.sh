# build & install libgeoda to ../deps/osx/geoda

echo "create R wrappers"
swig -c++ -r -I../../deps/osx/libgeoda/include -o libgeoda.cpp ../libgeoda.i

# create R package
rm -rf build
mkdir build
cp -rf pkg-template build/libgeoda
cp libgeoda.R build/libgeoda/R/
cp libgeoda.cpp build/libgeoda/src
mkdir build/libgeoda/src/deps
mkdir build/libgeoda/src/deps/osx
mkdir build/libgeoda/src/deps/osx/libgeoda
cp -rf ../../deps/osx/libgeoda/ build/libgeoda/src/deps/osx/libgeoda/
cd build

R CMD BUILD libgeoda
R CMD REMOVE libgeoda
R CMD INSTALL libgeoda_0.1.0.tar.gz

cd ..