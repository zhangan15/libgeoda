#!/usr/bin/env python
import sys, os
import setuptools
from distutils.core import setup, Extension

#PYTHON_HEADER = '-I/usr/include/python2.7'
OS_NAME = 'osx'

if sys.platform == "darwin":
    OS_NAME = 'osx'
    os.environ['MACOSX_DEPLOYMENT_TARGET'] = '10.14'

INCLUDE_DIRS = [
    './deps/include',
    './deps/libgeoda/include',
    './deps/lib/' + OS_NAME + '/wx/include/base-unicode-static-3.0'
]

LIBRARY_DIRS = ['/usr/lib']

LIBRARIES = [
       'curl',
       'iconv',
]

SWIG_OPTS = ['-c++']

EXTRA_COMPILE_ARGS = [
    '-D_FILE_OFFSET_BITS=64',
    '-std=c++11',
    '-mmacosx-version-min=10.9'
]

EXTRA_LINK_ARGS = [
       '-framework',
       'IOKit', 
       '-framework',
       'CoreServices',
       '-framework',
       'System',
       '-framework',
       'ApplicationServices',
       '-stdlib=libc++',
]

EXTRA_OBJECTS = [
    './deps/libgeoda/lib/' + OS_NAME + '/libgeoda.a',
    './deps/lib/' + OS_NAME + '/libANN.a',
    './deps/lib/' + OS_NAME + '/libgdal.a',
    './deps/lib/' + OS_NAME + '/libgeos.a',
    './deps/lib/' + OS_NAME + '/libgeos_c.a',
    './deps/lib/' + OS_NAME + '/libproj.a',
    './deps/lib/' + OS_NAME + '/libwx_baseu-3.0.a',
    './deps/lib/' + OS_NAME + '/libwxregexu-3.0.a',
    './deps/lib/' + OS_NAME + '/libboost_thread.a',
    './deps/lib/' + OS_NAME + '/libboost_system.a',
    './deps/lib/' + OS_NAME + '/libboost_date_time.a',
    './deps/lib/' + OS_NAME + '/libboost_chrono.a'
]

SOURCE_FILES  = [
    'pygeoda/libgeoda.cpp'
]
 
extensions = [Extension('pygeoda._libgeoda',
                        sources=SOURCE_FILES,
                        include_dirs=INCLUDE_DIRS,
                        swig_opts=SWIG_OPTS,
                        extra_compile_args=EXTRA_COMPILE_ARGS,
                        extra_link_args=EXTRA_LINK_ARGS,
                        library_dirs=LIBRARY_DIRS,
                        runtime_library_dirs=LIBRARY_DIRS,
                        libraries=LIBRARIES,
                        extra_objects=EXTRA_OBJECTS),]

setup (name = 'pygeoda',
       version = '0.0.1',
       author = "Xun Li",
       author_email = "lixun910@gmail.com",
       url = "https://github.com/lixun910/libgeoda",
       description = """Python wrapper for GeoDa""",
       ext_modules = extensions,
       packages=['pygeoda']
      )

