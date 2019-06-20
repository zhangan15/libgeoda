swig -c++ -r -I../../deps/libgeoda/include -o libgeoda.cpp ../libgeoda.i

set PKG_CPPFLAGS=-I../../deps/libgeoda/include -I../../deps/gdal/2.2.4/include -I../../deps/wx/3.1/include -I../../deps/wx/3.1/lib/mswu -D_FILE_OFFSET_BITS=64 
set PKG_LIBS=../../deps/libgeoda/lib/win64/geoda.lib ../../deps/gdal/2.2.4/lib/win64/gdal_i.lib ../../deps/wx/3.1/lib/wxmsw31u.lib ../../deps/wx/3.1/lib/wxmsw31u_gl.lib ../../deps/wx/3.1/lib/wxexpat.lib ../../deps/wx/3.1/lib/wxregexu.lib ../../deps/wx/3.1/lib/wxscintilla.lib ../../deps/wx/3.1/lib/wxtiff.lib ../../deps/wx/3.1/lib/wxpng.lib ../../deps/wx/3.1/lib/wxjpeg.lib ../../deps/wx/3.1/lib/wxzlib.lib  
R CMD SHLIB libgeoda.cpp

REM c:/Rtools/mingw_64/bin/g++ -shared -s -static-libgcc -o libgeoda.dll libgeoda.o C:/libgeoda/deps/iconv/lib/win64/libiconv.lib C:/libgeoda/deps/libgeoda/lib/win64/geoda.lib C:/libgeoda/deps/ann/lib/win64/libANN.lib C:/libgeoda/deps/gdal/2.2.4/lib/win64/gdal.lib C:/libgeoda/deps/wx/lib/win64/wxbase30u.lib C:/libgeoda/deps/wx/lib/win64/wxregexu.lib C:/libgeoda/deps/wx/lib/win64/wxscintilla.lib C:/libgeoda/deps/wx/lib/win64/wxzlib.lib C:/libgeoda/deps/geos/3.7.2/lib/win64/geos.lib C:/libgeoda/deps/boost/57/lib/win64/libboost_thread-vc100-mt-1_57.lib C:/libgeoda/deps/boost/57/lib/win64/libboost_system-vc100-mt-1_57.lib C:/libgeoda/deps/boost/57/lib/win64/libboost_chrono-vc100-mt-1_57.lib C:/libgeoda/deps/boost/57/lib/win64/libboost_date_time-vc100-mt-1_57.lib -LC:/PROGRA~1/R/R-36~1.0/bin/x64 -lR
REM cl /MD -I"C:/PROGRA~1/R/R-36~1.0/include" -I"C:\libgeoda\deps/libgeoda/include" -I"C:\libgeoda\deps/gdal/2.2.4/include" -I"C:\libgeoda\deps/ann/include" -I"C:\libgeoda\deps/geos/3.7.2/include" -I"C:\libgeoda\deps/wx/3.1/include" -I"C:\libgeoda\deps/wx/3.1/lib/mswu" -D_FILE_OFFSET_BITS=64  -O2 -Wall  -mtune=generic -c libgeoda.cpp -o libgeoda.o
REM cl /LD libgeoda.obj libiconv.lib libR.lib geoda.lib libANN.lib gdal.lib geos.lib wxmsw31u.lib wxmsw31u_gl.lib wxexpat.lib wxregexu.lib wxscintilla.lib wxzlib.lib wxtiff.lib wxpng.lib wxjpeg.lib libboost_date_time-vc100-mt-1_57.lib libboost_thread-vc100-mt-1_57.lib  /link /LIBPATH:C:\libgeoda\deps/iconv/lib/win64 /LIBPATH:C:\libgeoda\deps/libgeoda/lib/win64 /LIBPATH:C:\libgeoda\deps/ann/lib/win64 /LIBPATH:C:\libgeoda\deps/gdal/2.2.4/lib/win64 /LIBPATH:C:\libgeoda\deps/wx/3.1/lib /LIBPATH:C:\libgeoda\deps/geos/3.7.2/lib/win64 /LIBPATH:C:\libgeoda\deps/boost/57/lib/win64 /LIBPATH:C:/PROGRA~1/R/R-36~1.0/bin/x64 /out:libgeoda.dll 
REM link /DLL libgeoda.obj C:\libgeoda\deps/iconv/lib/win64/libiconv.lib C:\libgeoda\deps/libgeoda/lib/win64/geoda.lib C:\libgeoda\deps/ann/lib/win64/libANN.lib C:\libgeoda\deps/gdal/2.2.4/lib/win64/gdal.lib C:\libgeoda\deps/geos/3.7.2/lib/win64/geos.lib C:\libgeoda\deps/wx/lib/win64/wxbase30u.lib C:\libgeoda\deps/wx/lib/win64/wxexpat.lib C:\libgeoda\deps/wx/lib/win64/wxregexu.lib C:\libgeoda\deps/wx/lib/win64/wxscintilla.lib C:\libgeoda\deps/wx/lib/win64/wxzlib.lib C:\libgeoda\deps/boost/57/lib/win64/libboost_date_time-vc100-mt-1_57.lib C:\libgeoda\deps/boost/57/lib/win64/libboost_thread-vc100-mt-1_57.lib  C:/PROGRA~1/R/R-36~1.0/bin/x64/R.dll /out:libgeoda.dll 