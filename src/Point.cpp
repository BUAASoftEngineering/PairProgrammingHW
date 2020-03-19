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

// https://zh.cppreference.com/w/cpp/types/numeric_limits/epsilon

bool Coordinate::operator<(const Coordinate &other) const {
    return other.value - value > std::numeric_limits<double>::epsilon() * std::abs(value + other.value)
           || other.value - value > std::numeric_limits<double>::min();
}

bool Coordinate::operator==(const Coordinate &other) const {
    return !(*this < other) && !(*this > other);
}

bool Coordinate::operator>(const Coordinate &other) const {
    return value - other.value > std::numeric_limits<double>::epsilon() * std::abs(value + other.value)
           || value - other.value > std::numeric_limits<double>::min();
}


Point::Point(Coordinate xx, Coordinate yy) : x(xx), y(yy) {}

std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}
