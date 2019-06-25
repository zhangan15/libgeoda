
# libgeoda for R on Windows

tools and prerequisits: Windiws SDK V7, Visual studio Express 2010, R 3.6.0

## Prepare source code

Windows doesn't share the same symbolic link system as in Unix system. 

```
cd src
del geoda
mklink /D geoda ../../geoda
```


## Build libgeoda for R with Visual Studio 

### Create R.lib

TO link with R.dll, we need to create R.lib, which is not included in R installer. 

```
dumpbin /exports C:\Program Files\R\R-3.6.0\bin\x64\R.dll > R.exports
```

Then, based on `R.exports`, create a `R.def` file that defines the functions exported from DLL. For example:

```
LIBRARY R.dll
EXPORTS
R_RegisterCFinalizerEx
R_MakeExternalPtr
R_ExternalPtrAddr
R_NilValue
TYPEOF
Rf_protect
Rf_unprotect
SET_STRING_ELT
Rf_mkChar
```

Use `lib` command to create a lib file using R.def:
```
lib /def:R.def /OUT:R.lib /machine:x64
```

> The file R.def is located at `deps/R.def`; the file R.lib (x64) is located at `deps/lib/x64/R.lib`

### build libgeos 3.3.8
```asm

```

### build libproj 4.8.0

### build libcurl 7.46.0

### build iconv 1.14

### build GDAL 2.2.4

change nmake.opt file to configure a minimalized build of gdal library. 

>1>gdal.lib(vrtderivedrasterband.obj) : error LNK2019: unresolved external symbol EnumProcessModules referenced in function "bool __cdecl LoadPythonAPI(void)" (?LoadPythonAPI@@YA_NXZ)

1.  /DWIN32 has to be added so that proper header files could be included. `frmts/vrt/Makefile.vc`

build command:
```
nmake -f makefile.vc MSVC_VER=1600 WIN64=1
```

Note: if report undefined symbols from objects in apps/ directory, 
run the following command, then re-run above command:
```
nmake -f makefile.vc MSVC_VER=1600 WIN64=1 appslib-target
```

To build a static lib only:
```
nmake -f makefile.vc MSVC_VER=1600 WIN64=1 staticlib
```

Virtual Dataset driver: vrtderivedrasterband.cpp is implemented using Python C interface, so it has to be compiled by linking to Python library when using static library.

### build wxWidgets 3.0.4 (static linking)

```
nmake -f makefile.vc TARGET_CPU=AMD64 MONOLITHIC=1 USE_GUI=1 BUILD=release USE_HTML=0 USE_WEBVIEW=0 USE_MEDIA=0 USE_XRC=0 USE_AUI=0 USE_RIBBON=0 USE_PROPGRID=0 USE_RICHTEXT=0 USE_STC=0 USE_OPENGL=0 USE_QA=0  USE_RIBBON=0 USE_PROPGRID=0 USE_RICHTEXT=0 USE_STC=0 USE_OPENGL=0 USE_QA=0 USE_RTTI=0 USE_CAIRO=0
nmake -f makefile.vc MONOLITHIC=1 USE_GUI=1 BUILD=release USE_HTML=0 USE_WEBVIEW=0 USE_MEDIA=0 USE_XRC=0 USE_AUI=0 USE_RIBBON=0 USE_PROPGRID=0 USE_RICHTEXT=0 USE_STC=0 USE_OPENGL=0 USE_QA=0  USE_RIBBON=0 USE_PROPGRID=0 USE_RICHTEXT=0 USE_STC=0 USE_OPENGL=0 USE_QA=0 USE_RTTI=0 USE_CAIRO=0

```

### build Boost 57

```
b2 --with-thread --with-date_time --with-chrono --with-system --toolset=%MSVC++% --build-type=complete architecture=x86 address-model=64 stage

```

### build libgeoda static libray: geoda.lib

`msvc\` directory: geoda.sln


### build libgeoda.dll for R Cextension

The dependencies of libgeoda.dll include: geoda.lib, R.lib, gdal static lib, wxwidgets static lib etc.

> Note: comctl32.lib, rpcrt4.lib are for wxWidgets; and Psapi.lib is for gdal.lib
```
geoda.lib
R.lib
gdal.lib
geos.lib
proj.lib
libcurl_a.lib
libiconv.lib
wxmsw30u.lib
wxexpat.lib
wxregexu.lib
wxpng.lib
wxzlib.lib
comctl32.lib
rpcrt4.lib
Psapi.lib
```

> Note: wxWidgets and gdal's static libraries both have zlib, and it will cause duplicated symbols. The workaround 
is to add `/FORCE:MULTIPLE` option in  `Additional Options` of command line of Linker in visual studio.

## Test libgeoda for R

The dll name has to be "libgeoda", which is the R package name. Otherwise, calling functions in libgeoda will report "function can't be found" error.

```R
dyn.load("libgeoda.dll")
source("libgeoda.R")
cacheMetaData(1)

source("sf_geoda.R")
gda <- GeoDa("../../data/natregimes.shp")
```

## Package libgeoda on windows

The precompiled DLLs will be used directly so that no compilation will be needed 
, since it's too complex and may cause problem when installation. 

The dlls have to be placed under the directory `inst/libs`, and are structured as $(arch)/libgeoda.dll".

Besides, `.onLoad()` function should be implemented to load DLL manually based on the 
OS "windows" and architecture type "i386" or "x64". See file `AAA.R`.