//
// Created by xiangweilai on 2020/3/7.
//

#ifndef GEOMETRY_POINT_H
#define GEOMETRY_POINT_H

#include <cmath>
#include <ostream>
#include <iomanip>
#include <sstream>

typedef long long ll;
const int PRECISION = 6;
const double REV_EPS = 1e6;

class Coordinate {
public:
    double value;

    explicit Coordinate(double v_double);

    Coordinate() = default;

    bool operator<(const Coordinate &other) const;

    bool operator==(const Coordinate &other) const;

    bool operator>(const Coordinate &other) const;

    friend std::ostream &operator<<(std::ostream &os, const Coordinate &coordinate);

    std::size_t hashCode() const {
        return std::hash<long long>{}((long long) (value * REV_EPS));
    }
};


class Point {
public:
    Coordinate x, y;

    Point(Coordinate xx, Coordinate yy);

    Point() = default;

    friend std::ostream &operator<<(std::ostream &os, const Point &point);
};

// Hash Function For Point objects in support of unordered_map<Point>
struct hashCode_Point {
    std::size_t operator()(const Point &point) const {
        return point.x.hashCode() ^ (point.y.hashCode() << 1u);
    }
};

struct equals_Point {
    bool operator()(const Point &lhs, const Point &rhs) const {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};


#endif //GEOMETRY_POINT_H
