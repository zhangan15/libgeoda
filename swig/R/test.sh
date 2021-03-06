cp ../libgeoda.i rgeoda.i
sed -i.bu 's/module libgeoda/module rgeoda/g' rgeoda.i 
swig -c++ -r -I../../deps/libgeoda/include -o rgeoda.cpp ./rgeoda.i

CPP_FLAGS="-I/Library/Frameworks/R.framework/Resources/include -DNDEBUG" PKG_CPPFLAGS="-I./deps/libgeoda/include -I./deps/include  -I./deps/lib/osx/wx/include/base-unicode-static-3.0 -D_FILE_OFFSET_BITS=64" PKG_LIBS="-L/usr/lib -lcurl -liconv ./deps/libgeoda/lib/osx/libgeoda.a ./deps/lib/osx/libANN.a ./deps/lib/osx/libgdal.a ./deps/lib/osx/libwx_baseu-3.0.a ./deps/lib/osx/libwxregexu-3.0.a ./deps/lib/osx/libgeos.a ./deps/lib/osx/libgeos_c.a -framework IOKit -framework CoreServices -framework System -framework ApplicationServices ./deps/lib/osx/libboost_thread.a ./deps/lib/osx/libboost_system.a ./deps/lib/osx/libboost_chrono.a ./deps/lib/osx/libboost_date_time.a" R CMD SHLIB rgeoda.cpp
