
## Prepare source code

Windows doesn't share the same symbolic link system as in Unix system. 

```
cd src
del geoda
mklink /D geoda ../../geoda
```


## Build libgeoda for R with Visual Studio (2010 express is used)

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

### build libgeos
### build libproj
### build libcurl
### build iconv

### build GDAL 2.2.4

change nmake.opt file to configure a minimalized build of gdal library. 

>1>gdal.lib(vrtderivedrasterband.obj) : error LNK2019: unresolved external symbol EnumProcessModules referenced in function "bool __cdecl LoadPythonAPI(void)" (?LoadPythonAPI@@YA_NXZ)

1.  /DWIN32 has to be added so that proper header files could be included. `frmts/vrt/Makefile.vc`

build command:
```
nmake -f makefile MSVC_VER=1600 WIN64=1
```

Note: if report undefined symbols from objects in apps/ directory, 
run the following command, then re-run above command:
```
nmake -f makefile MSVC_VER=1600 WIN64=1 appslib-target
```

To build a static lib only:
```
nmake -f makefile.vc MSVC_VER=1600 WIN64=1 staticlib
```

Virtual Dataset driver: vrtderivedrasterband.cpp is implemented using Python C interface, so it has to be compiled by linking to Python library when using static library.

### build wxWidgets 3.0.4


### build Boost 57


## Test libgeoda for R

The dll name has to be "libgeoda", which is the R package name. Otherwise, calling functions in libgeoda will report "function can't be found" error.

```R
dyn.load("libgeoda.dll")
source("libgeoda.R")
cacheMetaData(1)

source("sf_geoda.R")
gda <- GeoDa("../../data/natregimes.shp"
```