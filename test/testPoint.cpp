//
// Created by xiangweilai on 2020/3/7.
//

#include <set>
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
    EXPECT_TRUE(c.isInf());
}

TEST(CoordinateTest, nan3) {
    Coordinate c(1, -2, 3, 0);
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "nan");
    EXPECT_TRUE(c.isInf());
}

TEST(CoordinateTest, operator_smaller) {
    Coordinate a(-1, 1e3);
    Coordinate b(1, 1e3);
    Coordinate c(-1, 1e8);
    Coordinate d(1, 1e8);
    Coordinate e(-0, 100);
    EXPECT_TRUE(a < c && c < e && e < d && d < b);
    Coordinate x(53315010, 1e8);
    Coordinate z(-53315012, -1e8);
    Coordinate i(2, 1, 3, 7);  // 0.533150115
    Coordinate j(4, 3, 5, 20); // 0.535
    Coordinate k(5, 3, 8, 25); // 0.539
    EXPECT_TRUE(x < i && i < z && z < j && j < k);
}

TEST(CoordinateTest, operator_larger) {
    Coordinate a(-1, 1e3);
    Coordinate b(1, 1e3);
    Coordinate c(-1, 1e8);
    Coordinate d(1, 1e8);
    Coordinate e(-0, 100);
    EXPECT_TRUE(c > a && e > c && d > e && b > d);
    Coordinate x(53315010, 1e8);
    Coordinate z(-53315012, -1e8);
    Coordinate i(2, 1, 3, 7);  // 0.533150115
    Coordinate j(4, 3, 5, 20); // 0.535
    Coordinate k(5, 3, 8, 25); // 0.539
    EXPECT_TRUE(i > x && z > i && j > z && k > j);
}

TEST(CoordinateTest, operator_equal) {
    Coordinate a(-1, 1e8);
    Coordinate b(1, 1e8);
    Coordinate c(-1, 1e3);
    Coordinate d(1, 1e3);
    Coordinate e(-0, 100);
    EXPECT_FALSE(c == a || e == c || d == e || b == d);
    Coordinate x(53315010, 1e8);
    Coordinate z(-53315012, -1e8);
    Coordinate i(2, 1, 3, 7);  // 0.533150115
    Coordinate j(4, 3, 5, 20); // 0.535
    Coordinate k(5, 3, 8, 25); // 0.539
    EXPECT_FALSE(i == x || z == i || j == z || k == j);
}

TEST(CoordinateTest, hashCode) {
    Coordinate a(-1, 1e8);
    Coordinate b(1, 1e8);
    Coordinate c(-1, 1e3);
    Coordinate d(1, 1e3);
    Coordinate e(-0, 100);
    Coordinate x(53315010, 1e8);
    Coordinate z(-53315012, -1e8);
    Coordinate i(2, 1, 3, 7);  // 0.533150115
    Coordinate j(4, 3, 5, 20); // 0.535
    Coordinate k(5, 3, 8, 25); // 0.539
    std::set<std::size_t> codes;
    Coordinate arr[] = {a, b, c, d, e, x, z, i, j, k};
    int count = 0, count1 = 0, count_rational = 0;
    for (auto ele: arr) {
        auto code = ele.hashCode();
        codes.insert(code);
        count++;
        if ((code & 1u) == 1)
            count1++;
        if (ele.isRational)
            count_rational++;
    }
    EXPECT_EQ(codes.size(), count);
    EXPECT_EQ(count1, count_rational);
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

TEST(OnLineTest, HalfLine1) {
    // y = 3x
    Line halfline(0, 0, 100000, 300000, LineType::HALF_LINE);
    // (0, 0)
    Coordinate x1(-0, 100);
    Coordinate y1(4, -2, 4, -4);
    bool res1 = halfline.checkPoint(x1, y1);
    // (100000, 300000)
    Coordinate x2(-100000, -1);
    Coordinate y2(300000, 1);
    bool res2 = halfline.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(1, -1e8);
    Coordinate y3(-3, 1e8);
    bool res3 = halfline.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(1, 1e8);
    Coordinate y4(3, 1e8);
    bool res4 = halfline.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(100000 * 1e3 - 1, 1e3);
    Coordinate y5(300000 * 1e3 - 3, 1e3);
    bool res5 = halfline.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(100000 * 1e3 + 1, 1e3);
    Coordinate y6(300000 * 1e3 + 3, 1e3);
    bool res6 = halfline.checkPoint(x6, y6);
    EXPECT_TRUE(res1);
    EXPECT_TRUE(res2);
    EXPECT_TRUE(!res3);
    EXPECT_TRUE(res4);
    EXPECT_TRUE(res5);
    EXPECT_TRUE(res6);
}

TEST(OnLineTest, HalfLine2) {
    // y = -3x
    Line halfline(0, 0, 100000, -300000, LineType::HALF_LINE);
    // (0, 0)
    Coordinate x1(-0, 100);
    Coordinate y1(4, -2, 4, -4);
    bool res1 = halfline.checkPoint(x1, y1);
    // (100000, -300000)
    Coordinate x2(-100000, -1);
    Coordinate y2(-300000, 1);
    bool res2 = halfline.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(1, -1e8);
    Coordinate y3(3, 1e8);
    bool res3 = halfline.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(1, 1e8);
    Coordinate y4(-3, 1e8);
    bool res4 = halfline.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(100000 * 1e3 - 1, 1e3);
    Coordinate y5(-300000 * 1e3 + 3, 1e3);
    bool res5 = halfline.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(100000 * 1e3 + 1, 1e3);
    Coordinate y6(-300000 * 1e3 - 3, 1e3);
    bool res6 = halfline.checkPoint(x6, y6);
    EXPECT_TRUE(res1);
    EXPECT_TRUE(res2);
    EXPECT_TRUE(!res3);
    EXPECT_TRUE(res4);
    EXPECT_TRUE(res5);
    EXPECT_TRUE(res6);
}

TEST(OnLineTest, HalfLine3) {
    // y = 3x， reverse
    Line halfline(100000, 300000, 0, 0, LineType::HALF_LINE);
    // (0, 0)
    Coordinate x1(-0, 100);
    Coordinate y1(4, -2, 4, -4);
    bool res1 = halfline.checkPoint(x1, y1);
    // (100000, 300000)
    Coordinate x2(-100000, -1);
    Coordinate y2(300000, 1);
    bool res2 = halfline.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(1, -1e8);
    Coordinate y3(-3, 1e8);
    bool res3 = halfline.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(1, 1e8);
    Coordinate y4(3, 1e8);
    bool res4 = halfline.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(100000 * 1e3 - 1, 1e3);
    Coordinate y5(300000 * 1e3 - 3, 1e3);
    bool res5 = halfline.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(100000 * 1e3 + 1, 1e3);
    Coordinate y6(300000 * 1e3 + 3, 1e3);
    bool res6 = halfline.checkPoint(x6, y6);
    EXPECT_TRUE(res1);
    EXPECT_TRUE(res2);
    EXPECT_TRUE(res3);
    EXPECT_TRUE(res4);
    EXPECT_TRUE(res5);
    EXPECT_TRUE(!res6);
}

TEST(OnLineTest, HalfLine4) {
    // y = -3x， reverse
    Line halfline(100000, -300000, 0, 0, LineType::HALF_LINE);
    // (0, 0)
    Coordinate x1(-0, 100);
    Coordinate y1(4, -2, 4, -4);
    bool res1 = halfline.checkPoint(x1, y1);
    // (100000, -300000)
    Coordinate x2(-100000, -1);
    Coordinate y2(-300000, 1);
    bool res2 = halfline.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(1, -1e8);
    Coordinate y3(3, 1e8);
    bool res3 = halfline.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(1, 1e8);
    Coordinate y4(-3, 1e8);
    bool res4 = halfline.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(100000 * 1e3 - 1, 1e3);
    Coordinate y5(-300000 * 1e3 + 3, 1e3);
    bool res5 = halfline.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(100000 * 1e3 + 1, 1e3);
    Coordinate y6(-300000 * 1e3 - 3, 1e3);
    bool res6 = halfline.checkPoint(x6, y6);
    EXPECT_TRUE(res1);
    EXPECT_TRUE(res2);
    EXPECT_TRUE(res3);
    EXPECT_TRUE(res4);
    EXPECT_TRUE(res5);
    EXPECT_TRUE(!res6);
}

TEST(OnLineTest, Seg1) {
    // y = 3x
    Line seg(0, 0, 100000, 300000, LineType::SEGMENT_LINE);
    // (0, 0)
    Coordinate x1(-0, 100);
    Coordinate y1(4, -2, 4, -4);
    bool res1 = seg.checkPoint(x1, y1);
    // (100000, 300000)
    Coordinate x2(-100000, -1);
    Coordinate y2(300000, 1);
    bool res2 = seg.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(1, -1e8);
    Coordinate y3(-3, 1e8);
    bool res3 = seg.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(1, 1e8);
    Coordinate y4(3, 1e8);
    bool res4 = seg.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(100000 * 1e3 - 1, 1e3);
    Coordinate y5(300000 * 1e3 - 3, 1e3);
    bool res5 = seg.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(100000 * 1e3 + 1, 1e3);
    Coordinate y6(300000 * 1e3 + 3, 1e3);
    bool res6 = seg.checkPoint(x6, y6);
    EXPECT_TRUE(res1);
    EXPECT_TRUE(res2);
    EXPECT_TRUE(!res3);
    EXPECT_TRUE(res4);
    EXPECT_TRUE(res5);
    EXPECT_TRUE(!res6);
}

TEST(OnLineTest, Seg2) {
    // y = -3x
    Line seg(0, 0, 100000, -300000, LineType::SEGMENT_LINE);
    // (0, 0)
    Coordinate x1(-0, 100);
    Coordinate y1(4, -2, 4, -4);
    bool res1 = seg.checkPoint(x1, y1);
    // (100000, -300000)
    Coordinate x2(-100000, -1);
    Coordinate y2(-300000, 1);
    bool res2 = seg.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(1, -1e8);
    Coordinate y3(3, 1e8);
    bool res3 = seg.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(1, 1e8);
    Coordinate y4(-3, 1e8);
    bool res4 = seg.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(100000 * 1e3 - 1, 1e3);
    Coordinate y5(-300000 * 1e3 + 3, 1e3);
    bool res5 = seg.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(100000 * 1e3 + 1, 1e3);
    Coordinate y6(-300000 * 1e3 - 3, 1e3);
    bool res6 = seg.checkPoint(x6, y6);
    EXPECT_TRUE(res1);
    EXPECT_TRUE(res2);
    EXPECT_TRUE(!res3);
    EXPECT_TRUE(res4);
    EXPECT_TRUE(res5);
    EXPECT_TRUE(!res6);
}

TEST(OnLineTest, Seg3) {
    // y = 3x， reverse
    Line seg(100000, 300000, 0, 0, LineType::SEGMENT_LINE);
    // (0, 0)
    Coordinate x1(-0, 100);
    Coordinate y1(4, -2, 4, -4);
    bool res1 = seg.checkPoint(x1, y1);
    // (100000, 300000)
    Coordinate x2(-100000, -1);
    Coordinate y2(300000, 1);
    bool res2 = seg.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(1, -1e8);
    Coordinate y3(-3, 1e8);
    bool res3 = seg.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(1, 1e8);
    Coordinate y4(3, 1e8);
    bool res4 = seg.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(100000 * 1e3 - 1, 1e3);
    Coordinate y5(300000 * 1e3 - 3, 1e3);
    bool res5 = seg.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(100000 * 1e3 + 1, 1e3);
    Coordinate y6(300000 * 1e3 + 3, 1e3);
    bool res6 = seg.checkPoint(x6, y6);
    EXPECT_TRUE(res1);
    EXPECT_TRUE(res2);
    EXPECT_TRUE(!res3);
    EXPECT_TRUE(res4);
    EXPECT_TRUE(res5);
    EXPECT_TRUE(!res6);
}

TEST(OnLineTest, Seg4) {
    // y = -3x， reverse
    Line seg(100000, -300000, 0, 0, LineType::SEGMENT_LINE);
    // (0, 0)
    Coordinate x1(-0, 100);
    Coordinate y1(4, -2, 4, -4);
    bool res1 = seg.checkPoint(x1, y1);
    // (100000, -300000)
    Coordinate x2(-100000, -1);
    Coordinate y2(-300000, 1);
    bool res2 = seg.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(1, -1e8);
    Coordinate y3(3, 1e8);
    bool res3 = seg.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(1, 1e8);
    Coordinate y4(-3, 1e8);
    bool res4 = seg.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(100000 * 1e3 - 1, 1e3);
    Coordinate y5(-300000 * 1e3 + 3, 1e3);
    bool res5 = seg.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(100000 * 1e3 + 1, 1e3);
    Coordinate y6(-300000 * 1e3 - 3, 1e3);
    bool res6 = seg.checkPoint(x6, y6);
    EXPECT_TRUE(res1);
    EXPECT_TRUE(res2);
    EXPECT_TRUE(!res3);
    EXPECT_TRUE(res4);
    EXPECT_TRUE(res5);
    EXPECT_TRUE(!res6);
}
