//
// Created by xiangweilai on 2020/3/7.
//

#include <set>
#include "gtest/gtest.h"
#include "Point.h"
#include "Shapes.h"

inline double rational(ll top, ll bottom) {
    return (double) top / (double) bottom;
}

inline double irrational(ll a, ll b, ll c, ll btm) {
    return (a + sqrt(c) * b) / btm;
}

TEST(CoordinateTest, construct1) {
    Coordinate c(rational(1, 2));
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "0.500000");
}

TEST(CoordinateTest, construct2) {
    Coordinate c(irrational(1, 2, 3, 4));
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "1.116025");
}

TEST(CoordinateTest, simplify1) {
    Coordinate c(irrational(1, 2, 9, 4));
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "1.750000");
}

TEST(CoordinateTest, simplify2) {
    Coordinate c(irrational(1, -3, 9, 4));
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "-2.000000");
}

TEST(CoordinateTest, simplify3) {
    Coordinate c(irrational(1, 2, 12, 4));
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "1.982051");
}

TEST(CoordinateTest, simplify4) {
    Coordinate c(rational(40000000000, 60000000000));
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "0.666667");
}

TEST(CoordinateTest, simplify5) {
    Coordinate c(rational(40000000000, -60000000000));
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "-0.666667");
}

TEST(CoordinateTest, simplify6) {
    Coordinate c(rational(-40000000000, -60000000000));
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "0.666667");
}

TEST(CoordinateTest, simplify7) {
    Coordinate c(rational(-40000000000, 60000000000));
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "-0.666667");
}

TEST(CoordinateTest, simplify8) {
    Coordinate c(irrational(2, 3, 12, 4));
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "3.098076");
}

TEST(CoordinateTest, simplify9) {
    Coordinate c(irrational(-2, -3, 12, -4));
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "3.098076");
}

TEST(CoordinateTest, nan1) {
    Coordinate c(rational(10000000000, 9999999999));
    std::ostringstream outstream;
    outstream << c;
    EXPECT_EQ(outstream.str(), "1.000000");
}

TEST(CoordinateTest, operator_smaller) {
    Coordinate a(rational(-1, 1e3));
    Coordinate b(rational(1, 1e3));
    Coordinate c(rational(-1, 1e5));
    Coordinate d(rational(1, 1e5));
    Coordinate e(rational(-0, 100));
    EXPECT_TRUE(a < c && c < e && e < d && d < b);
    Coordinate x(rational(53314, 1e5));
    Coordinate z(rational(-53316, -1e5));
    Coordinate i(irrational(2, 1, 3, 7));  // 0.533150115
    Coordinate j(irrational(4, 3, 5, 20)); // 0.535
    Coordinate k(irrational(5, 3, 8, 25)); // 0.539
    EXPECT_TRUE(x < i && i < z && z < j && j < k);
}

TEST(CoordinateTest, operator_larger) {
    Coordinate a(rational(-1, 1e3));
    Coordinate b(rational(1, 1e3));
    Coordinate c(rational(-1, 1e5));
    Coordinate d(rational(1, 1e5));
    Coordinate e(rational(-0, 100));
    EXPECT_TRUE(c > a && e > c && d > e && b > d);
    Coordinate x(rational(53314, 1e5));
    Coordinate z(rational(-53316, -1e5));
    Coordinate i(irrational(2, 1, 3, 7));  // 0.533150115
    Coordinate j(irrational(4, 3, 5, 20)); // 0.535
    Coordinate k(irrational(5, 3, 8, 25)); // 0.539
    EXPECT_TRUE(i > x && z > i && j > z && k > j);
}

TEST(CoordinateTest, operator_equal) {
    Coordinate a(rational(-1, 1e5));
    Coordinate b(rational(1, 1e5));
    Coordinate c(rational(-1, 1e3));
    Coordinate d(rational(1, 1e3));
    Coordinate e(rational(-0, 100));
    EXPECT_FALSE(c == a || e == c || d == e || b == d);
    Coordinate x(rational(53314, 1e5));
    Coordinate z(rational(-53316, -1e5));
    Coordinate i(irrational(2, 1, 3, 7));  // 0.533150115
    Coordinate j(irrational(4, 3, 5, 20)); // 0.535
    Coordinate k(irrational(5, 3, 8, 25)); // 0.539
    EXPECT_FALSE(i == x || z == i || j == z || k == j);
}

TEST(CoordinateTest, hashCode) {
    Coordinate a(rational(-1, 1e5));
    Coordinate b(rational(1, 1e5));
    Coordinate c(rational(-1, 1e3));
    Coordinate d(rational(1, 1e3));
    Coordinate e(rational(-0, 100));
    Coordinate x(rational(53314, 1e5));
    Coordinate z(rational(-53316, -1e5));
    Coordinate i(irrational(2, 1, 3, 7));  // 0.533150115
    Coordinate j(irrational(4, 3, 5, 20)); // 0.535
    Coordinate k(irrational(5, 3, 8, 25)); // 0.539
    std::set<std::size_t> codes;
    Coordinate arr[] = {a, b, c, d, e, x, z, i, j, k};
    int count = 0;
    for (auto ele: arr) {
        auto code = ele.hashCode();
        codes.insert(code);
        count++;
    }
    EXPECT_EQ(codes.size(), count);
}

TEST(PointTest, construct) {
    Coordinate x(rational(6, 8));
    Coordinate y(irrational(1, -2, 3, -4));
    Point p(x, y);
    std::ostringstream outstream;
    outstream << p;
    EXPECT_EQ(outstream.str(), "(0.750000, 0.616025)");
}

TEST(OnLineTest, Line) {
    // y = 3x
    Line line(-100000, -300000, 100000, 300000);
    // (0, 0)
    Coordinate x1(rational(-0, 100));
    Coordinate y1(irrational(4, -2, 4, -4));
    bool res1 = line.checkPoint(x1, y1);
    // (100000, 300000)
    Coordinate x2(rational(-100000, -1));
    Coordinate y2(rational(300000, 1));
    bool res2 = line.checkPoint(x2, y2);
    // (-100000, -300000)
    Coordinate x3(rational(100000, -1));
    Coordinate y3(rational(-300000, 1));
    bool res3 = line.checkPoint(x3, y3);
    // (1e-8, 3e-8)
    Coordinate x4(rational(1, 1e8));
    Coordinate y4(rational(-3, -1e8));
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
    Coordinate x1(rational(-0, 100));
    Coordinate y1(irrational(4, -2, 4, -4));
    bool res1 = halfline.checkPoint(x1, y1);
    // (100000, 300000)
    Coordinate x2(rational(-100000, -1));
    Coordinate y2(rational(300000, 1));
    bool res2 = halfline.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(rational(1, -1e5));
    Coordinate y3(rational(-3, 1e5));
    bool res3 = halfline.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(rational(1, 1e5));
    Coordinate y4(rational(3, 1e5));
    bool res4 = halfline.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(rational(100000 * 1e3 - 1, 1e3));
    Coordinate y5(rational(300000 * 1e3 - 3, 1e3));
    bool res5 = halfline.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(rational(100000 * 1e3 + 1, 1e3));
    Coordinate y6(rational(300000 * 1e3 + 3, 1e3));
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
    Coordinate x1(rational(-0, 100));
    Coordinate y1(irrational(4, -2, 4, -4));
    bool res1 = halfline.checkPoint(x1, y1);
    // (100000, -300000)
    Coordinate x2(rational(-100000, -1));
    Coordinate y2(rational(-300000, 1));
    bool res2 = halfline.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(rational(1, -1e5));
    Coordinate y3(rational(3, 1e5));
    bool res3 = halfline.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(rational(1, 1e5));
    Coordinate y4(rational(-3, 1e5));
    bool res4 = halfline.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(rational(100000 * 1e3 - 1, 1e3));
    Coordinate y5(rational(-300000 * 1e3 + 3, 1e3));
    bool res5 = halfline.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(rational(100000 * 1e3 + 1, 1e3));
    Coordinate y6(rational(-300000 * 1e3 - 3, 1e3));
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
    Coordinate x1(rational(-0, 100));
    Coordinate y1(irrational(4, -2, 4, -4));
    bool res1 = halfline.checkPoint(x1, y1);
    // (100000, 300000)
    Coordinate x2(rational(-100000, -1));
    Coordinate y2(rational(300000, 1));
    bool res2 = halfline.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(rational(1, -1e5));
    Coordinate y3(rational(-3, 1e5));
    bool res3 = halfline.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(rational(1, 1e5));
    Coordinate y4(rational(3, 1e5));
    bool res4 = halfline.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(rational(100000 * 1e3 - 1, 1e3));
    Coordinate y5(rational(300000 * 1e3 - 3, 1e3));
    bool res5 = halfline.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(rational(100000 * 1e3 + 1, 1e3));
    Coordinate y6(rational(300000 * 1e3 + 3, 1e3));
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
    Coordinate x1(rational(-0, 100));
    Coordinate y1(irrational(4, -2, 4, -4));
    bool res1 = halfline.checkPoint(x1, y1);
    // (100000, -300000)
    Coordinate x2(rational(-100000, -1));
    Coordinate y2(rational(-300000, 1));
    bool res2 = halfline.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(rational(1, -1e5));
    Coordinate y3(rational(3, 1e5));
    bool res3 = halfline.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(rational(1, 1e5));
    Coordinate y4(rational(-3, 1e5));
    bool res4 = halfline.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(rational(100000 * 1e3 - 1, 1e3));
    Coordinate y5(rational(-300000 * 1e3 + 3, 1e3));
    bool res5 = halfline.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(rational(100000 * 1e3 + 1, 1e3));
    Coordinate y6(rational(-300000 * 1e3 - 3, 1e3));
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
    Coordinate x1(rational(-0, 100));
    Coordinate y1(irrational(4, -2, 4, -4));
    bool res1 = seg.checkPoint(x1, y1);
    // (100000, 300000)
    Coordinate x2(rational(-100000, -1));
    Coordinate y2(rational(300000, 1));
    bool res2 = seg.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(rational(1, -1e5));
    Coordinate y3(rational(-3, 1e5));
    bool res3 = seg.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(rational(1, 1e5));
    Coordinate y4(rational(3, 1e5));
    bool res4 = seg.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(rational(100000 * 1e3 - 1, 1e3));
    Coordinate y5(rational(300000 * 1e3 - 3, 1e3));
    bool res5 = seg.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(rational(100000 * 1e3 + 1, 1e3));
    Coordinate y6(rational(300000 * 1e3 + 3, 1e3));
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
    Coordinate x1(rational(-0, 100));
    Coordinate y1(irrational(4, -2, 4, -4));
    bool res1 = seg.checkPoint(x1, y1);
    // (100000, -300000)
    Coordinate x2(rational(-100000, -1));
    Coordinate y2(rational(-300000, 1));
    bool res2 = seg.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(rational(1, -1e5));
    Coordinate y3(rational(3, 1e5));
    bool res3 = seg.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(rational(1, 1e5));
    Coordinate y4(rational(-3, 1e5));
    bool res4 = seg.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(rational(100000 * 1e3 - 1, 1e3));
    Coordinate y5(rational(-300000 * 1e3 + 3, 1e3));
    bool res5 = seg.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(rational(100000 * 1e3 + 1, 1e3));
    Coordinate y6(rational(-300000 * 1e3 - 3, 1e3));
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
    Coordinate x1(rational(-0, 100));
    Coordinate y1(irrational(4, -2, 4, -4));
    bool res1 = seg.checkPoint(x1, y1);
    // (100000, 300000)
    Coordinate x2(rational(-100000, -1));
    Coordinate y2(rational(300000, 1));
    bool res2 = seg.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(rational(1, -1e5));
    Coordinate y3(rational(-3, 1e5));
    bool res3 = seg.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(rational(1, 1e5));
    Coordinate y4(rational(3, 1e5));
    bool res4 = seg.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(rational(100000 * 1e3 - 1, 1e3));
    Coordinate y5(rational(300000 * 1e3 - 3, 1e3));
    bool res5 = seg.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(rational(100000 * 1e3 + 1, 1e3));
    Coordinate y6(rational(300000 * 1e3 + 3, 1e3));
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
    Coordinate x1(rational(-0, 100));
    Coordinate y1(irrational(4, -2, 4, -4));
    bool res1 = seg.checkPoint(x1, y1);
    // (100000, -300000)
    Coordinate x2(rational(-100000, -1));
    Coordinate y2(rational(-300000, 1));
    bool res2 = seg.checkPoint(x2, y2);
    // (0-, )
    Coordinate x3(rational(1, -1e5));
    Coordinate y3(rational(3, 1e5));
    bool res3 = seg.checkPoint(x3, y3);
    // (0+, )
    Coordinate x4(rational(1, 1e5));
    Coordinate y4(rational(-3, 1e5));
    bool res4 = seg.checkPoint(x4, y4);
    // (100000-, )
    Coordinate x5(rational(100000 * 1e3 - 1, 1e3));
    Coordinate y5(rational(-300000 * 1e3 + 3, 1e3));
    bool res5 = seg.checkPoint(x5, y5);
    // (100000+, )
    Coordinate x6(rational(100000 * 1e3 + 1, 1e3));
    Coordinate y6(rational(-300000 * 1e3 - 3, 1e3));
    bool res6 = seg.checkPoint(x6, y6);
    EXPECT_TRUE(res1);
    EXPECT_TRUE(res2);
    EXPECT_TRUE(!res3);
    EXPECT_TRUE(res4);
    EXPECT_TRUE(res5);
    EXPECT_TRUE(!res6);
}
