//
// Created by xiangweilai on 2020/3/7.
//

#include <iostream>
#include "gtest/gtest.h"
#include "Point.h"
#include "Shapes.h"

TEST(CoordinateTest, construct1) {
    Coordinate c(1, 2);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "1 / 2");
}

TEST(CoordinateTest, construct2) {
    Coordinate c(1, 2, 3, 4);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "1.11602540");
}

TEST(CoordinateTest, simplify1) {
    Coordinate c(1, 2, 9, 4);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "7 / 4");
}

TEST(CoordinateTest, simplify2) {
    Coordinate c(1, -3, 9, 4);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "-2 / 1");
}

TEST(CoordinateTest, simplify3) {
    Coordinate c(1, 2, 12, 4);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "1.98205081");
}

TEST(CoordinateTest, simplify4) {
    Coordinate c(40000000000, 60000000000);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "2 / 3");
}

TEST(CoordinateTest, simplify5) {
    Coordinate c(40000000000, -60000000000);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "-2 / 3");
}

TEST(CoordinateTest, simplify6) {
    Coordinate c(-40000000000, -60000000000);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "2 / 3");
}

TEST(CoordinateTest, simplify7) {
    Coordinate c(-40000000000, 60000000000);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "-2 / 3");
}

TEST(CoordinateTest, simplify8) {
    Coordinate c(2, 3, 12, 4);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "3.09807621");
}

TEST(CoordinateTest, simplify9) {
    Coordinate c(-2, -3, 12, -4);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "3.09807621");
}

TEST(CoordinateTest, nan1) {
    Coordinate c(10000000000, 9999999999);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "10000000000 / 9999999999");
}

TEST(CoordinateTest, nan2) {
    Coordinate c(-10000000000, 00000000);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "nan");
}

TEST(CoordinateTest, nan3) {
    Coordinate c(1, -2, 3, 0);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "nan");
}


TEST(PointTest, construct) {
    Coordinate x(6, 8);
    Coordinate y(1, -2, 3, -4);
    Point p(x, y);
    std::ostringstream outstream;
    outstream << p;
    EXPECT_EQ(outstream.str(), "(3 / 4, 0.61602540)");
}

TEST(OnLineTest, Line) {
    // y = 3x
    Line line(-100000, -300000, 100000, 300000);
    // (0, 0)
    Coordinate x1(-0, 100);
    Coordinate y1(4, -2, 4, -4);
    bool res1 = line.checkPoint(x1, y1);
    // (100000, 300000)
    Coordinate x2(-100000, -1);
    Coordinate y2(300000, 1);
    bool res2 = line.checkPoint(x2, y2);
    // (-100000, -300000)
    Coordinate x3(100000, -1);
    Coordinate y3(-300000, 1);
    bool res3 = line.checkPoint(x3, y3);
    // (1e-8, 3e-8)
    Coordinate x4(1, 1e8);
    Coordinate y4(-3, -1e8);
    bool res4 = line.checkPoint(x4, y4);
    EXPECT_TRUE(res1);
    EXPECT_TRUE(res2);
    EXPECT_TRUE(res3);
    EXPECT_TRUE(res4);
}

TEST(OnLineTest, HalfLine) {
    // y = 3x
    HalfLine halfline(0, 0, 100000, 300000);
    // (0, 0)
    Coordinate x1(-0, 100);
    Coordinate y1(4, -2, 4, -4);
    bool res1 = halfline.checkPoint(x1, y1);
    // (100000, 300000)
    Coordinate x2(-100000, -1);
    Coordinate y2(300000, 1);
    bool res2 = halfline.checkPoint(x2, y2);
    // (-100000, -300000)
    Coordinate x3(100000, -1);
    Coordinate y3(-300000, 1);
    bool res3 = halfline.checkPoint(x3, y3);
    // (1e-8, 3e-8)
    Coordinate x4(1, 1e8);
    Coordinate y4(-3, -1e8);
    bool res4 = halfline.checkPoint(x4, y4);
    // (-1e-8, -3e-8)
    Coordinate x5(-1, 1e8);
    Coordinate y5(3, -1e8);
    bool res5 = halfline.checkPoint(x5, y5);
    // (1e8, 3e8)
    Coordinate x6(1e8, 1);
    Coordinate y6(-3e8, -1);
    bool res6 = halfline.checkPoint(x6, y6);
    // (-1e8, -3e8)
    Coordinate x7(-1e8, 1);
    Coordinate y7(3e8, -1);
    bool res7 = halfline.checkPoint(x7, y7);
    EXPECT_TRUE(res1);
    EXPECT_TRUE(res2);
    EXPECT_TRUE(!res3);
    EXPECT_TRUE(res4);
    EXPECT_TRUE(!res5);
    EXPECT_TRUE(res6);
    EXPECT_TRUE(!res7);
}