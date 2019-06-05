#!/usr/bin/env python

from distutils.core import setup, Extension

DEP_DIR = '../../deps/osx/'
SRC_DIR = '../../src/'
#PYTHON_HEADER = '-I/usr/include/python2.7'

INCLUDE_DIRS = [
       SRC_DIR, 
       DEP_DIR + 'ann/include',
       DEP_DIR + 'gdal/include',
       DEP_DIR + 'geos/3.7.2/include',
       DEP_DIR + 'wx/include/wx-3.0',
       DEP_DIR + 'wx/lib/wx/include/base-unicode-static-3.0',
       '/usr/local/include'
]

LIBRARY_DIRS = ['/usr/lib']

LIBRARIES = [
       'curl',
       'iconv',
]

SWIG_OPTS = ['-c++']

EXTRA_COMPILE_ARGS = [
       '-D_FILE_OFFSET_BITS=64',
]

EXTRA_LINK_ARGS = [
       '-framework',
       'IOKit', 
       '-framework',
       'CoreServices',
       '-framework',
       'System',
       '-framework',
       'ApplicationServices'
]

EXTRA_OBJECTS = [
       DEP_DIR + 'ann/lib/libANN.a',
       DEP_DIR + 'gdal/lib/libgdal.a',
       DEP_DIR + 'geos/3.7.2/lib/libgeos.a',
       DEP_DIR + 'geos/3.7.2/lib/libgeos_c.a',
       DEP_DIR + 'wx/lib/libwx_baseu-3.0.a',
       DEP_DIR + 'wx/lib/libwxregexu-3.0.a',
       '/usr/local/lib/libboost_thread-mt.a',
       '/usr/local/lib/libboost_system-mt.a',
       '/usr/local/lib/libboost_chrono-mt.a',
       '/usr/local/lib/libboost_date_time-mt.a',
       '/usr/local/lib/libboost_atomic-mt.a',
       ]

SOURCE_FILES  = [
       'geoda_proxy_wrap.cxx',
       SRC_DIR + 'libgeoda.cpp'
]
 
extensions = [Extension('_libgeoda',
                        sources=SOURCE_FILES,
                        include_dirs=INCLUDE_DIRS,
                        swig_opts=SWIG_OPTS,
                        extra_compile_args=EXTRA_COMPILE_ARGS,
                        extra_link_args=EXTRA_LINK_ARGS,
                        library_dirs=LIBRARY_DIRS,
                        runtime_library_dirs=LIBRARY_DIRS,
                        libraries=LIBRARIES,
                        extra_objects=EXTRA_OBJECTS),]

setup (name = 'GeoDa',
       version = '0.1',
       author = "Xun Li",
       description = """Python wrapper for GeoDa""",
       ext_modules = extensions,
       py_modules = ["geoda"],
      )

