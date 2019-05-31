#cmake_minimum_required(VERSION 3.9)
cmake_minimum_required(VERSION 2.8)

project(geoda)

###########################################################
#
# variables
#
###########################################################

message(STATUS "Binary dir: ${CMAKE_BINARY_DIR}")

set(CMAKE_MACOSX_RPATH 1)

set(DEP_LIBRARY_PATH)
if ( APPLE )
    set (DEP_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/deps/osx)
endif()

#set (CMAKE_IGNORE_PATH "/usr/local/include")
set (CMAKE_OSX_ARCHITECTURES "x86_64")
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -arch x86_64")

# gtest
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

###########################################################
#
# dependencies
#
###########################################################
## libcurl
find_package(CURL REQUIRED)

## ANN library
set (ANN_INCLUDE_DIR ${DEP_LIBRARY_PATH}/ann/include)
set (ANN_LIBRARIES ${DEP_LIBRARY_PATH}/ann/lib/libANN.a)

## Boost: static link
set (Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS thread system REQUIRED)

## wxWidgets library (non-ui)
set (WX_INCLUDE_DIR ${DEP_LIBRARY_PATH}/wx/include/wx-3.0 ${DEP_LIBRARY_PATH}/wx/lib/wx/include/base-unicode-static-3.0)
#set (CMAKE_CXX_FLAGS  ${CMAKE_CXX_FLAGS} "-D_FILE_OFFSET_BITS=64")
# CMAKE_MODULE_LINKER_FLAGS
# CMAKE_SHARED_LINKER_FLAGS
# CMAKE_STATIC_LINKER_FLAGS
if ( APPLE )
    set (WX_LIBRARIES ${DEP_LIBRARY_PATH}/wx/lib/libwx_baseu-3.0.a ${DEP_LIBRARY_PATH}/wx/lib/libwxregexu-3.0.a)
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -framework IOKit -framework CoreServices -framework System -framework ApplicationServices")
endif()

## gdal library (minimal)
set (GDAL_INCLUDE_DIR ${DEP_LIBRARY_PATH}/gdal/include)
set (GDAL_LIBRARIES ${DEP_LIBRARY_PATH}/gdal/lib/libgdal.a)

## geos library 
set (GEOS_LIBRARIES ${DEP_LIBRARY_PATH}/geos/3.7.2/lib/libgeos.a ${DEP_LIBRARY_PATH}/geos/3.7.2/lib/libgeos_c.a)

###########################################################
#
# output files and flags
#
###########################################################
add_library(geoda SHARED src/library.cpp )


# Setup output for testing swig
set_target_properties( geoda
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/swig"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/swig"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/swig"
        )

# Setup public headers for libgeoda
set_target_properties(geoda PROPERTIES PUBLIC_HEADER "src/library.h")

# Add install target for libgeoda
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include)

# PRIVATE - use only for this specified build target
# PUBLIC - use it for specified target and for targets which links with this project
# INTERFACE -- use it only for targets which links with the current project

target_include_directories(geoda PUBLIC ${CURL_INCLUDE_DIR})
target_link_libraries(geoda ${CURL_LIBRARIES})

target_include_directories(geoda PUBLIC ${GDAL_INCLUDE_DIR} )
target_link_libraries(geoda ${GDAL_LIBRARIES})

target_link_libraries(geoda ${GEOS_LIBRARIES})

target_include_directories(geoda PUBLIC ${ANN_INCLUDE_DIR} )
target_link_libraries(geoda ${ANN_LIBRARIES} )

target_include_directories(geoda PUBLIC ${Boost_INCLUDE_DIR} )
target_link_libraries(geoda ${Boost_LIBRARIES})

target_include_directories(geoda PUBLIC ${WX_INCLUDE_DIR} )
target_link_libraries(geoda ${WX_LIBRARIES})
if (UNIX)
    target_link_libraries(geoda iconv)
endif()

###########################################################
#
#  testing
#
###########################################################
# Add swig subdirectory
#add_subdirectory(swig)


################################
# GTest
################################
ADD_SUBDIRECTORY (googletest)
enable_testing()


include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


################################
# Unit Tests
################################
# Add test cpp file
add_executable( runUnitTests test/testgtest.cpp)

set_target_properties(runUnitTests PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(runUnitTests PUBLIC ${geoda_SOURCE_DIR}/src)

# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main geoda)
add_test( runUnitTests runUnitTests )
