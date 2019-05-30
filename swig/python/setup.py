#!/usr/bin/env python

from distutils.core import setup, Extension

#PYTHON_HEADER = '-I/usr/include/python2.7'

INCLUDE_DIRS = ['../../src/', '../']

LIBRARY_DIRS = ['../']

LIBRARIES = ['geoda']

SWIG_OPTS = ['-c++']

EXTRA_COMPILE_ARGS = []

EXTRA_LINK_ARGS = []

EXTRA_OBJECTS = []

SOURCE_FILES  = ['geoda_proxy_wrap.cxx',
                 '../geoda_proxy.cpp']
 
extensions = [Extension('_geoda',
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

