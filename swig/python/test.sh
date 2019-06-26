swig -python -c++ -threads -I../../deps/libgeoda/include  -o pygeoda/libgeoda.cpp ../libgeoda.i

CFLAGS='-Wall -O0 -DDEBUG' python setup.py build_ext --inplace --force

mv _

