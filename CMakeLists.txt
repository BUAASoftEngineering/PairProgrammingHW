cmake_minimum_required(VERSION 3.15)
project(Geometry)

set(CMAKE_CXX_STANDARD 17)
set(GOOGLETEST_VERSION 1.10.0) # from googletest-master/CML.txt

add_executable(Geometry src/main.cpp src/Point.cpp src/Point.h src/Shapes.cpp src/Shapes.h) # main

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

if ("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang"
        OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    message("Building with llvm Code Coverage Tools")
    set(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
elseif (${CMAKE_COMPILER_IS_GNUCXX})
    message("Building with gcov Code Coverage Tools")
    set(CMAKE_CXX_FLAGS "--coverage")
endif ()