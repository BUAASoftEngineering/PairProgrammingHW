//
// Created by xiangweilai on 2020/3/7.
//

#include "Point.h"


Coordinate::Coordinate(double v_double) : value(v_double) {}

std::ostream &operator<<(std::ostream &os, const Coordinate &coordinate) {
    os << std::fixed << std::setprecision(PRECISION) << coordinate.value;
    return os;
}

// for small numbers, use fixed eps = BASE_EPS * DYNAMIC_EPS_START
// for large numbers, use dynamic eps = BASE_EPS * value

bool Coordinate::operator<(const Coordinate &other) const {
    double scale = std::min(std::abs(value), std::abs(other.value));
    if (scale >= DYNAMIC_EPS_START)
        return other.value - value > BASE_EPS * scale;              // dynamic eps
    else
        return other.value - value > BASE_EPS * DYNAMIC_EPS_START;  // fixed eps, continuous at the border
}

bool Coordinate::operator==(const Coordinate &other) const {
    return !(*this < other) && !(*this > other);
}

bool Coordinate::operator>(const Coordinate &other) const {
    double scale = std::min(std::abs(value), std::abs(other.value));
    if (scale >= DYNAMIC_EPS_START)
        return value - other.value > BASE_EPS * scale;              // dynamic eps
    else
        return value - other.value > BASE_EPS * DYNAMIC_EPS_START;  // fixed eps, continuous at the border
}


Point::Point(Coordinate xx, Coordinate yy) : x(xx), y(yy) {}

std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

int GLOBAL_COLLISION_COUNT = 0;
int GLOBAL_HASH_COUNT = 0;