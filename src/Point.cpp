//
// Created by xiangweilai on 2020/3/7.
//

#include "Point.h"


Coordinate::Coordinate(double v_double) : value(v_double) {}

std::ostream &operator<<(std::ostream &os, const Coordinate &coordinate) {
    os << std::fixed << std::setprecision(PRECISION) << coordinate.value;
    return os;
}

bool Coordinate::operator<(const Coordinate &other) const {
    return value + EPS < other.value;
}

bool Coordinate::operator==(const Coordinate &other) const {
    return !(*this < other) && !(*this > other);
}

bool Coordinate::operator>(const Coordinate &other) const {
    return value > EPS + other.value;
}


Point::Point(Coordinate xx, Coordinate yy) : x(xx), y(yy) {}

std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}
