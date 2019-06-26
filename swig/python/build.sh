swig -python -c++ -threads -I../../deps/libgeoda/include  -o pygeoda/libgeoda.cpp ../libgeoda.i

rm -rf package
mkdir package
cd package
cp -rf ../pygeoda .
cp ../setup.py .
mkdir deps
mkdir deps/lib
cp -rf ../../../deps/include deps/
rm -rf deps/include/boost/
cp -rf ../../../deps/lib/osx deps/lib/
cp -rf ../../../deps/libgeoda deps/


python setup.py bdist_wheel
MACOSX_DEPLOYMENT_TARGET=10.9 python3 setup.py bdist_wheel
python setup.py sdist
#twine upload dist/*

cd ..