//
// Created by xiangweilai on 2020/3/10.
//

#include <iostream>
#include "gtest/gtest.h"
#include "Shapes.h"

TEST(IntersectionTest, line_line1) {
    // y = x & y = -x
    Line line1(-100000, -100000, 100000, 100000);
    Line line2(100000, -100000, -100000, 100000);
    auto result = intersection(line1, line2).first;
    EXPECT_EQ(result.size(), 1);
    Point point = result[0];
    std::ostringstream outstream;
    outstream << point;
    EXPECT_EQ(outstream.str(), "(0.000000, 0.000000)");
}

TEST(IntersectionTest, line_line2) {
    // y = x & y = -1.2x - 0.4
    Line line1(-100000, -100000, 100000, 100000);
    Line line2(-2, 2, 3, -4);
    auto result = intersection(line1, line2).first;
    EXPECT_EQ(result.size(), 1);
    Point point = result[0];
    std::ostringstream outstream;
    outstream << point;
    EXPECT_EQ(outstream.str(), "(-0.181818, -0.181818)");
}

TEST(IntersectionTest, line_line3) {
    // y = x & y = 1e5
    Line line1(-100000, -100000, 100000, 100000);
    Line line2(-100000, 100000, 100000, 100000);
    auto result = intersection(line1, line2).first;
    EXPECT_EQ(result.size(), 1);
    Point point = result[0];
    std::ostringstream outstream;
    outstream << point;
    EXPECT_EQ(outstream.str(), "(100000.000000, 100000.000000)");
}

TEST(IntersectionTest, line_line4) {
    // y = x & x = -1e5
    Line line1(-100000, -100000, 100000, 100000);
    Line line2(-100000, -100000, -100000, 100000);
    auto result = intersection(line1, line2).first;
    EXPECT_EQ(result.size(), 1);
    Point point = result[0];
    std::ostringstream outstream;
    outstream << point;
    EXPECT_EQ(outstream.str(), "(-100000.000000, -100000.000000)");
}

TEST(IntersectionTest, line_line5) {
    // y = 0 & x = -10
    Line line1(-1, 0, 0, 0);
    Line line2(-10, -5, -10, 5);
    auto result = intersection(line1, line2).first;
    EXPECT_EQ(result.size(), 1);
    Point point = result[0];
    std::ostringstream outstream;
    outstream << point;
    EXPECT_EQ(outstream.str(), "(-10.000000, 0.000000)");
}

TEST(IntersectionTest, line_line6) {
    // y = 2x & y = 2x + 1
    Line line1(-1, -2, 2, 4);
    Line line2(10, 21, 100, 201);
    auto result = intersection(line1, line2).first;
    EXPECT_EQ(result.size(), 0);
}

TEST(IntersectionTest, line_line7) {
    // y = 0 & y = -1
    Line line1(100, 0, -2, 0);
    Line line2(10, -1, 100, -1);
    auto result = intersection(line1, line2).first;
    EXPECT_EQ(result.size(), 0);
}

TEST(IntersectionTest, line_line8) {
    // x = 0 & x = -1
    Line line1(0, 0, 0, 1);
    Line line2(-1, 1, -1, -1);
    auto result = intersection(line1, line2).first;
    EXPECT_EQ(result.size(), 0);
}

TEST(IntersectionTest, line_line9) {
    // random test
    Line line1(237, -968, -326, -484);
    Line line2(-638, -352, 783, -776);
    auto result = intersection(line1, line2).first;
    EXPECT_EQ(result.size(), 1);
    Point point = result[0];
    std::ostringstream outstream;
    outstream << point;
    EXPECT_EQ(outstream.str(), "(-395.312445, -424.413458)");
}


std::string getOutput(Point x) {
    std::ostringstream outstream;
    outstream << x;
    return outstream.str();
}


TEST(IntersectionTest, line_circle1) {
    // x^2 + y^2 = 1 & x = 0
    Line line(0, -100000, 0, 100000);
    Circle circle(0, 0, 1);
    auto result = intersection(line, circle).first;
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(getOutput(result[0]), "(0.000000, 1.000000)");
    EXPECT_EQ(getOutput(result[1]), "(0.000000, -1.000000)");
}

TEST(IntersectionTest, line_circle2) {
    // x^2 + y^2 = 1 & y = 0
    Line line(-100000, 0, 100000, 0);
    Circle circle(0, 0, 1);
    auto result = intersection(circle, line).first;
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(getOutput(result[0]), "(1.000000, 0.000000)");
    EXPECT_EQ(getOutput(result[1]), "(-1.000000, 0.000000)");
}

TEST(IntersectionTest, line_circle3) {
    // x^2 + y^2 = 1 & y = x
    Line line(0, 0, 100000, 100000);
    Circle circle(0, 0, 1);
    auto result = intersection(line, circle).first;
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(getOutput(result[0]), "(0.707107, 0.707107)");
    EXPECT_EQ(getOutput(result[1]), "(-0.707107, -0.707107)");
}

TEST(IntersectionTest, line_circle4) {
    // x^2 + y^2 = 1 & y = x + 1
    Line line(0, 1, 100000, 100001);
    Circle circle(0, 0, 1);
    auto result = intersection(circle, line).first;
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(getOutput(result[0]), "(0.000000, 1.000000)");
    EXPECT_EQ(getOutput(result[1]), "(-1.000000, 0.000000)");
}

TEST(IntersectionTest, line_circle5) {
    // x^2 + y^2 = 1 & y = 2x - 1
    Line line(0, -1, 1, 1);
    Circle circle(0, 0, 1);
    auto result = intersection(line, circle).first;
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(getOutput(result[0]), "(0.800000, 0.600000)");
    EXPECT_EQ(getOutput(result[1]), "(0.000000, -1.000000)");
}

TEST(IntersectionTest, line_circle6) {
    // x^2 + y^2 = 1 & y = 1
    Line line(0, 1, 1, 1);
    Circle circle(0, 0, 1);
    auto result = intersection(circle, line).first;
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(getOutput(result[0]), "(0.000000, 1.000000)");
}

TEST(IntersectionTest, line_circle7) {
    // x^2 + y^2 = 1 & x = -1
    Line line(-1, 2, -1, 1);
    Circle circle(0, 0, 1);
    auto result = intersection(line, circle).first;
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(getOutput(result[0]), "(-1.000000, 0.000000)");
}

TEST(IntersectionTest, line_circle8) {
    // random test
    Line line(-11, 6, -13, 1);
    Circle circle(-17, 4, 20);
    auto result = intersection(circle, line).first;
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(getOutput(result[0]), "(-5.309108, 20.227231)");
    EXPECT_EQ(getOutput(result[1]), "(-19.725375, -15.813438)");
}

TEST(IntersectionTest, line_circle9) {
    // random test
    Line line(-11, 6, -13, 1);
    Circle circle(-17, 4, 4);
    auto result = intersection(circle, line).first;
    EXPECT_EQ(result.size(), 0);
}

// Generic Tangent Cases ???

TEST(IntersectionTest, circle_circle1) {
    // inner / outer
    Circle circle1(0, 0, 2);
    Circle circle2(0, 0, 1);
    auto result = intersection(circle1, circle2).first;
    EXPECT_EQ(result.size(), 0);
}

TEST(IntersectionTest, circle_circle2) {
    // inner / outer
    Circle circle1(0, 0, 3);
    Circle circle2(1, 1, 1);
    auto result = intersection(circle1, circle2).first;
    EXPECT_EQ(result.size(), 0);
}

TEST(IntersectionTest, circle_circle3) {
    // inner tangent
    Circle circle1(0, 0, 3);
    Circle circle2(0, 2, 1);
    auto result = intersection(circle1, circle2).first;
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(getOutput(result[0]), "(0.000000, 3.000000)");
}

TEST(IntersectionTest, circle_circle4) {
    // outer tangent
    Circle circle1(0, 4, 3);
    Circle circle2(3, 0, 2);
    auto result = intersection(circle1, circle2).first;
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(getOutput(result[0]), "(1.800000, 1.600000)");
}

TEST(IntersectionTest, circle_circle5) {
    // inter
    Circle circle1(0, 0, 5);
    Circle circle2(3, 2, 6);
    auto result = intersection(circle1, circle2).first;
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(getOutput(result[0]), "(3.000000, -4.000000)");
    EXPECT_EQ(getOutput(result[1]), "(-2.538462, 4.307692)");
}

TEST(IntersectionTest, circle_circle6) {
    // random test
    Circle circle1(3, -5, 2);
    Circle circle2(-1, 1, 6);
    auto result = intersection(circle1, circle2).first;
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(getOutput(result[0]), "(3.429881, -3.046746)");
    EXPECT_EQ(getOutput(result[1]), "(1.031657, -4.645562)");
}
