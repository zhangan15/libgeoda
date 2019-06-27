swig -python -c++ -threads -I./deps/libgeoda/include  -o pygeoda/libgeoda.cpp ../libgeoda.i

LDFLAGS='-arch x86_64' CFLAGS='-Wall -O0 -DDEBUG' MACOSX_DEPLOYMENT_TARGET=10.14 python3 setup.py build_ext --inplace --force

mv pygeoda/_libgeoda.*.so pygeoda/_libgeoda.so