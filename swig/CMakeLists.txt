cmake_minimum_required(VERSION 2.8)


# Create main and link to libgeoda
add_library(swig geoda_proxy.cpp)

target_include_directories(swig PUBLIC ${geoda_SOURCE_DIR}/src)
target_link_libraries(swig geoda)

# Set up public headers for swig
set_target_properties(swig PROPERTIES PUBLIC_HEADER "geoda_proxy.h")