swig -c++ -r -outcurrentdir -o geoda_proxy_wrap.cpp ../geoda_proxy.i

PKG_CXXFLAGS="-I../  -I../../src" PKG_LIBS="-L../ -lgeoda" R CMD SHLIB -o geoda.so geoda_proxy_wrap.cpp ../geoda_proxy.cpp

install_name_tool -add_rpath @loader_path/. geoda.so
install_name_tool -add_rpath @loader_path/../ geoda.so
