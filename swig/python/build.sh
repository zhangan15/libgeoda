swig -python -c++ -threads -outcurrentdir ../geoda_proxy.i

CFLAGS='-Wall -O0 -DDEBUG' python setup.py build_ext --inplace --force