#cmake_minimum_required(VERSION 3.9)
cmake_minimum_required(VERSION 2.8)

project(geoda)

###########################################################
#
# variables
#
###########################################################

message(STATUS "Binary dir: ${CMAKE_BINARY_DIR}")

set (DEP_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/deps)
set (DEP_OS_SUB "")
set (GEODA_SRC "src/geoda")
if ( APPLE )
    set(CMAKE_MACOSX_RPATH 1)
    set (DEP_OS_SUB "osx")
    set (CMAKE_OSX_ARCHITECTURES "x86_64")
    set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -arch x86_64")

elseif (UNIX)
    set (DEP_OS_SUB "bionic")
    set (CMAKE_CXX_FLAGS  "-fPIC -Wl,--no-as-needed")

elseif (MSVC)
    set (DEP_OS_SUB "win64")
    set (GEODA_SRC "src/geoda_win")
endif()

#ADD_DEFINITIONS("__LIBGEODA__=1")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__LIBGEODA__=1")


###########################################################
#
# dependencies
#
###########################################################
## libcurl
if (APPLE)
    find_package(CURL REQUIRED)
endif()

## ANN library
set (ANN_INCLUDE_DIR ${DEP_LIBRARY_PATH}/ann/include)
set (ANN_LIBRARIES ${DEP_LIBRARY_PATH}/ann/lib/${DEP_OS_SUB}/libANN.a)

## Boost: static link
set (Boost_INCLUDE_DIR ${DEP_LIBRARY_PATH}/boost/include)
set (Boost_LIBRARIES
    ${DEP_LIBRARY_PATH}/boost/lib/${DEP_OS_SUB}/libboost_thread-mt.a
    ${DEP_LIBRARY_PATH}/boost/lib/${DEP_OS_SUB}/libboost_system-mt.a
    ${DEP_LIBRARY_PATH}/boost/lib/${DEP_OS_SUB}/libboost_chrono-mt.a
    ${DEP_LIBRARY_PATH}/boost/lib/${DEP_OS_SUB}/libboost_date_time-mt.a
)

## wxWidgets library (non-ui)
set (WX_INCLUDE_DIR
        ${DEP_LIBRARY_PATH}/wx/include/wx-3.0
        ${DEP_LIBRARY_PATH}/wx/lib/${DEP_OS_SUB}/wx/include/base-unicode-static-3.0)
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64")
set (WX_LIBRARIES
        ${DEP_LIBRARY_PATH}/wx/lib/${DEP_OS_SUB}/libwx_baseu-3.0.a
        ${DEP_LIBRARY_PATH}/wx/lib/${DEP_OS_SUB}/libwxregexu-3.0.a)
if (APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -framework IOKit -framework CoreServices -framework System -framework ApplicationServices")
endif()

## gdal library (minimal)
set (GDAL_INCLUDE_DIR ${DEP_LIBRARY_PATH}/gdal/2.2.4/include)
set (GDAL_LIBRARIES ${DEP_LIBRARY_PATH}/gdal/2.2.4/lib/${DEP_OS_SUB}/libgdal.a)

## geos library
#set (GEOS_INCLUDE_DIR ${DEP_LIBRARY_PATH}/geos/3.7.2/include)
set (GEOS_INCLUDE_DIR ${DEP_LIBRARY_PATH}/geos/3.7.2/include/geos)
set (GEOS_LIBRARIES
        ${DEP_LIBRARY_PATH}/geos/3.7.2/lib/${DEP_OS_SUB}/libgeos_c.a
        ${DEP_LIBRARY_PATH}/geos/3.7.2/lib/${DEP_OS_SUB}/libgeos.a)

set (PROJ4_LIBRARIES ${DEP_LIBRARY_PATH}/proj/5.2.0/lib/${DEP_OS_SUB}/libproj.a)

###########################################################
#
# output files and flags
#
###########################################################
set (GEODA_INCLUDE_FILES
        src/libgeoda.h
        src/AbstractLocalSA.h
        src/UniLisa.h
        ${GEODA_SRC}/ShapeOperations/GalWeight.h
        ${GEODA_SRC}/ShapeOperations/GeodaWeight.h
        ${GEODA_SRC}/ShapeOperations/WeightsManState.h
)

set (GEODA_SOURCE_FILES
        src/libgeoda.cpp
        ${GEODA_SRC}/logger.cpp
        ${GEODA_SRC}/GenUtils.cpp
        ${GEODA_SRC}/SpatialIndAlgs.cpp
        ${GEODA_SRC}/GenGeomAlgs.cpp
        ${GEODA_SRC}/ShapeOperations/VoronoiUtils.cpp
        ${GEODA_SRC}/ShapeOperations/GeodaWeight.cpp
        ${GEODA_SRC}/ShapeOperations/GwtWeight.cpp
        ${GEODA_SRC}/ShapeOperations/GalWeight.cpp
        ${GEODA_SRC}/ShapeOperations/PolysToContigWeights.cpp
        ${GEODA_SRC}/ShapeOperations/WeightsManState.cpp
        ${GEODA_SRC}/Algorithms/cluster.cpp
        ${GEODA_SRC}/Algorithms/redcap.cpp
        src/AbstractLocalSA.cpp
        src/UniLisa.cpp
)

add_library(geoda STATIC ${GEODA_SOURCE_FILES})

# Setup public headers for libgeoda
#set_target_properties(geoda PROPERTIES PUBLIC_HEADER ${GEODA_INCLUDE_FILES})
#set_target_properties(geoda PROPERTIES  CXX_STANDARD 11  CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)


# Add install target for libgeoda
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include)

# PRIVATE - use only for this specified build target
# PUBLIC - use it for specified target and for targets which links with this project
# INTERFACE -- use it only for targets which links with the current project

if (APPLE)
    target_include_directories(geoda PUBLIC ${CURL_INCLUDE_DIR})
    target_link_libraries(geoda ${CURL_LIBRARIES})
endif()

#target_include_directories(geoda PUBLIC ${GEOS_INCLUDE_DIR} )
#target_link_libraries(geoda ${GEOS_LIBRARIES})

target_include_directories(geoda PUBLIC ${GDAL_INCLUDE_DIR} )
target_link_libraries(geoda ${GDAL_LIBRARIES})

target_include_directories(geoda PUBLIC ${ANN_INCLUDE_DIR} )
target_link_libraries(geoda ${ANN_LIBRARIES} )

target_include_directories(geoda PUBLIC ${Boost_INCLUDE_DIR} )
target_link_libraries(geoda ${Boost_LIBRARIES})

target_include_directories(geoda PUBLIC ${WX_INCLUDE_DIR} )
target_link_libraries(geoda ${WX_LIBRARIES})

if (APPLE)
    target_link_libraries(geoda iconv)
elseif(UNIX)
    target_link_libraries(geoda "${DEP_LIBRARY_PATH}/iconv/lib/${DEP_OS_SUB}/libiconv.a")
endif()


#  For SWIG and testing only, copy files
foreach(hfile ${GEODA_INCLUDE_FILES})
    get_filename_component(barename ${hfile} NAME)
    add_custom_command(
            TARGET geoda POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/${hfile}
            ${CMAKE_SOURCE_DIR}/deps/libgeoda/include/${barename})
endforeach()
add_custom_command(
    TARGET geoda POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/libgeoda.a
    ${CMAKE_SOURCE_DIR}/deps/libgeoda/lib/${DEP_OS_SUB}/libgeoda.a)

###########################################################
#
#  googletest testing framework
#
###########################################################
# git clone https://github.com/google/googletest.git
ADD_SUBDIRECTORY (googletest)
enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


################################
# Unit Tests
################################
# Add test cpp file
set (TEST_SOURCE_FILES
        test/testgtest.cpp
        test/test_weights.cpp
        test/test_localSA.cpp
        test/test_clustering.cpp
        test/test_geometry.cpp test/test_table.cpp)
add_executable( runUnitTests ${TEST_SOURCE_FILES} )

set_target_properties(runUnitTests PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if (UNIX AND NOT APPLE)
    set_property(TARGET runUnitTests APPEND_STRING PROPERTY LINK_FLAGS " -Wl,--no-as-needed ")
endif()

target_include_directories(runUnitTests PUBLIC
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        ${geoda_SOURCE_DIR}/src
        ${geoda_SOURCE_DIR}/${GEODA_SRC}/ShapeOperations
)
# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main geoda)

if (APPLE)
    target_include_directories(runUnitTests PUBLIC ${CURL_INCLUDE_DIR})
    target_link_libraries(runUnitTests ${CURL_LIBRARIES})
    target_link_libraries(runUnitTests iconv)
endif()

target_link_libraries(runUnitTests ${GDAL_LIBRARIES})
target_link_libraries(runUnitTests ${GEOS_LIBRARIES})
target_link_libraries(runUnitTests ${PROJ4_LIBRARIES})

if (APPLE)
    find_library(CoreServices CoreServices)
    find_library(IOKit IOKit)
    find_library(System System)
    find_library(ApplicationServices ApplicationServices)
    target_link_libraries(runUnitTests
            ${IOKit}
            ${CoreServices}
            ${System}
            ${ApplicationServices}
    )
elseif(UNIX)
    target_link_libraries(runUnitTests dl)
endif()

add_test( runUnitTests runUnitTests )
