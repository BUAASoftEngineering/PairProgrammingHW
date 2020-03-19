//
// Created by xiangweilai on 2020/3/7.
//

#include "Point.h"
#include <limits>


Coordinate::Coordinate(double v_double) : value(v_double) {}

std::ostream &operator<<(std::ostream &os, const Coordinate &coordinate) {
    os << std::fixed << std::setprecision(PRECISION) << coordinate.value;
    return os;
}

// suppose that for 1.0, eps = BASE_EPS
// then for 100.0, eps = BASE_EPS * 10^2
// then for 10000.0, eps = BASE_EPS * 10^4

bool Coordinate::operator<(const Coordinate &other) const {
    return other.value - value > BASE_EPS * std::max(1.0, std::min(std::abs(value), std::abs(other.value)));
}

bool Coordinate::operator==(const Coordinate &other) const {
    return !(*this < other) && !(*this > other);
}

bool Coordinate::operator>(const Coordinate &other) const {
    return value - other.value > BASE_EPS * std::max(1.0, std::min(std::abs(value), std::abs(other.value)));
}


Point::Point(Coordinate xx, Coordinate yy) : x(xx), y(yy) {}

std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}
