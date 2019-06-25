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

elseif (WIN32)
    set (DEP_OS_SUB "win64")
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
set (ANN_INCLUDE_DIR ${DEP_LIBRARY_PATH}/include)
set (ANN_LIBRARIES ${DEP_LIBRARY_PATH}/lib/${DEP_OS_SUB}/libANN.a)
set (GEOS_INCLUDE_DIR ${DEP_LIBRARY_PATH}/include/geos)

if (UNIX)
    set (Boost_LIBRARIES
            ${DEP_LIBRARY_PATH}/lib/${DEP_OS_SUB}/libboost_thread.a
            ${DEP_LIBRARY_PATH}/lib/${DEP_OS_SUB}/libboost_system.a
            ${DEP_LIBRARY_PATH}/lib/${DEP_OS_SUB}/libboost_chrono.a
            ${DEP_LIBRARY_PATH}/lib/${DEP_OS_SUB}/libboost_date_time.a)
    set (WX_INCLUDE_DIR
            ${DEP_LIBRARY_PATH}/lib/${DEP_OS_SUB}/wx/include/base-unicode-static-3.0)
    set (WX_LIBRARIES
            ${DEP_LIBRARY_PATH}/lib/${DEP_OS_SUB}/libwx_baseu-3.0.a
            ${DEP_LIBRARY_PATH}/lib/${DEP_OS_SUB}/libwxregexu-3.0.a)
    if (APPLE)
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -framework IOKit -framework CoreServices -framework System -framework ApplicationServices")
    endif()
    set (GDAL_LIBRARIES ${DEP_LIBRARY_PATH}/lib/${DEP_OS_SUB}/libgdal.a)
    set (GEOS_LIBRARIES
            ${DEP_LIBRARY_PATH}/lib/${DEP_OS_SUB}/libgeos_c.a
            ${DEP_LIBRARY_PATH}/lib/${DEP_OS_SUB}/libgeos.a)
    set (PROJ4_LIBRARIES ${DEP_LIBRARY_PATH}/lib/${DEP_OS_SUB}/libproj.a)

elseif (WIN32)
    set (Boost_LIBRARIES
            libboost_date_time-vc100-mt-1_57.lib
            libboost_thread-vc100-mt-1_57.lib)
    set (WX_INCLUDE_DIR ${DEP_LIBRARY_PATH}/lib/${DEP_OS_SUB}/mswu)
    set (WX_LIBRARIES
        wxbase30u.lib
        wxexpat.lib
        wxregexu.lib
        wxscintilla.lib
        wxzlib.lib)
    set (GDAL_LIBRARIES gdal_i.lib)
    set (GEOS_LIBRARIES libgeos.lib)
    set (PROJ4_LIBRARIES proj.lib)
    link_directories(${DEP_LIBRARY_PATH}/lib)

endif()
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64")

message(status ${WX_INCLUDE_DIR})

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

add_library(geoda SHARED ${GEODA_SOURCE_FILES})

# Setup public headers for libgeoda
#set_target_properties(geoda PROPERTIES PUBLIC_HEADER ${GEODA_INCLUDE_FILES})
#set_target_properties(geoda PROPERTIES  CXX_STANDARD 11  CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
set_target_properties(geoda PROPERTIES  
    CXX_STANDARD 11 
    CXX_STANDARD_REQUIRED YES 
    CXX_EXTENSIONS NO)

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
    target_link_libraries(geoda iconv)
endif()

target_include_directories(geoda PUBLIC ${ANN_INCLUDE_DIR} )
target_include_directories(geoda PUBLIC ${WX_INCLUDE_DIR} )
target_include_directories(geoda PUBLIC ${GEOS_INCLUDE_DIR} )

target_link_libraries(geoda ${GEOS_LIBRARIES})
target_link_libraries(geoda ${GDAL_LIBRARIES})
target_link_libraries(geoda ${ANN_LIBRARIES} )
target_link_libraries(geoda ${Boost_LIBRARIES})
target_link_libraries(geoda ${WX_LIBRARIES})

if(UNIX AND NOT APPLE)
    target_link_libraries(geoda "${DEP_LIBRARY_PATH}/lib/${DEP_OS_SUB}/libiconv.a")
endif()

if (MSVC)
    target_link_libraries(geoda kernel32.lib)
endif()

if (UNIX)
    #  For SWIG and testing only, copy files
    foreach(hfile ${GEODA_INCLUDE_FILES})
    get_filename_component(barename ${hfile} NAME)
    add_custom_command(
            TARGET geoda POST_BUILD
            COMMAND ${CMAKE_COMMAND} ARGS -E copy
            ${CMAKE_SOURCE_DIR}/${hfile}
            ${CMAKE_SOURCE_DIR}/deps/libgeoda/include/${barename})
    endforeach()
    add_custom_command(
    TARGET geoda POST_BUILD
    COMMAND ${CMAKE_COMMAND} ARGS -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/libgeoda.a
    ${CMAKE_SOURCE_DIR}/deps/libgeoda/lib/${DEP_OS_SUB}/libgeoda.a)
endif()


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
