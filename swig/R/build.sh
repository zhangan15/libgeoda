# build & install libgeoda to ../deps/osx/geoda

echo "create R wrappers..."
cp ../libgeoda.i rgeoda.i
sed -i.bu 's/module libgeoda/module rgeoda/g' rgeoda.i 
swig -c++ -r -I../../deps/libgeoda/include -o rgeoda.cpp ./rgeoda.i

# create R package
rm -rf build
mkdir build
cp -rf pkg-template/ build/rgeoda

mkdir build/rgeoda/src/deps
mkdir build/rgeoda/src/deps/lib
cp -rf ../../deps/libgeoda/ build/rgeoda/src/deps/libgeoda
cp -rf ../../deps/lib/osx build/rgeoda/src/deps/lib
cp -rf ../../deps/include build/rgeoda/src/deps/include
rm -rf build/rgeoda/src/deps/include/boost/

cp rgeoda.R build/rgeoda/R/
cp sf_geoda.R build/rgeoda/R/
cp read_geoda.R build/rgeoda/R/
cp AAA.R build/rgeoda/R/

cp rgeoda.cpp build/rgeoda/src

cp *.Rmd build/rgeoda/docs
cp *.html build/rgeoda/docs

cd build

R CMD build rgeoda
R CMD REMOVE rgeoda
R CMD INSTALL rgeoda_0.1.0.tar.gz

cd ..
