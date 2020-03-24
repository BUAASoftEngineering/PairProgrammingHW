cmake_minimum_required(VERSION 3.15)

# export .dll.a to dynamic .lib
# see https://cmake.org/cmake/help/v3.1/prop_tgt/GNUtoMS.html
set(CMAKE_GNUtoMS on)

project(Combine)

set(CMAKE_CXX_STANDARD 17)
set(GOOGLETEST_VERSION 1.10.0) # from googletest-master/CML.txt


# General Settings: lib & include
link_directories(${PROJECT_SOURCE_DIR})
include_directories(src)


find_library(GCORE gCore HINTS ${PROJECT_SOURCE_DIR}/bin)
if (${GCORE} STREQUAL "GCORE-NOTFOUND")
    message("not found GCORE variable")
endif ()
message("libgCore path: ${GCORE}")


# main Executable Program (gCore)
add_executable(Combine src/main.cpp)
target_link_libraries(Combine ${GCORE})
