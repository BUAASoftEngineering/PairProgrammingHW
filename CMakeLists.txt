cmake_minimum_required(VERSION 3.15)

# export .dll.a to dynamic .lib
# see https://cmake.org/cmake/help/v3.1/prop_tgt/GNUtoMS.html
set(CMAKE_GNUtoMS on)

project(Geometry)

set(CMAKE_CXX_STANDARD 17)
set(GOOGLETEST_VERSION 1.10.0) # from googletest-master/CML.txt

add_library(libgeometry SHARED src/Point.cpp src/Point.h src/Shapes.cpp src/Shapes.h src/interface.h src/interface.cpp)
add_executable(Geometry src/main.cpp) # main
link_directories(${PROJECT_SOURCE_DIR})
target_link_libraries(Geometry libgeometry)

add_subdirectory(external/googletest)
include_directories(src
        external/googletest/include)

# Test: class Coordinate & class Point
add_executable(TestPoint
        test/testPoint.cpp
        src/Point.cpp
        src/Shapes.cpp
        external/googletest)
target_link_libraries(TestPoint gtest_main)

# Test: class Line & class Circle & variant Geometry & intersections<Shape1, Shape2>
add_executable(TestShapes
        test/testShapes.cpp
        src/Shapes.cpp
        src/Point.cpp
        external/googletest)
target_link_libraries(TestShapes gtest_main)

# Test: core module (libgeometry)
add_executable(TestCore
        test/testCore.cpp
        external/googletest)
target_link_libraries(TestCore gtest_main)
target_link_libraries(TestCore libgeometry)

# Test: exceptions (libgeometry)
add_executable(TestExceptions
        test/testExceptions.cpp
        external/googletest)
target_link_libraries(TestExceptions gtest_main)
target_link_libraries(TestExceptions libgeometry)

# Test: run all tests
add_executable(RunAllTests
        test/testPoint.cpp
        test/testShapes.cpp
        test/testCore.cpp
        test/testExceptions.cpp
        external/googletest)
target_link_libraries(RunAllTests gtest_main)
target_link_libraries(RunAllTests libgeometry)


if ("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang"
        OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    message("Building with llvm Code Coverage Tools")
    set(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
elseif (${CMAKE_COMPILER_IS_GNUCXX})
    message("Building with gcov Code Coverage Tools")
    set(CMAKE_CXX_FLAGS "--coverage")
endif ()