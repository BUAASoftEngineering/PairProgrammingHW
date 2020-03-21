//
// Created by xiangweilai on 2020/3/9.
//

#include "Shapes.h"

Line::Line(int x1, int y1, int x2, int y2) : p1_x(x1), p1_y(y1), p2_x(x2), p2_y(y2),
                                             type(LineType::LINE) {}

Line::Line(int x1, int y1, int x2, int y2, LineType type) :
        p1_x(x1), p1_y(y1), p2_x(x2), p2_y(y2), type(type) {}

bool Line::checkPoint(const Coordinate &x, const Coordinate &y) const {
    if (type == LineType::HALF_LINE) {
        return checkPointHalf(x, y, *this);
    } else if (type == LineType::SEGMENT_LINE) {
        return checkPointSegment(x, y, *this);
    }
    return true;
}


Circle::Circle(int x, int y, int r) : center_x(x), center_y(y), radius(r) {}


inline void _pushPoint(point_container_t &container, Coordinate &x, Coordinate &y) {
    const int size = (++std::get<1>(container));
    if (size == 1)
        std::get<2>(container) = Point(x, y);
    else // if (size == 2)
        std::get<3>(container) = Point(x, y);
}


point_container_t intersection(const Line &a, const Circle &b) {
//    std::cout << "Line - Circle!\n";
    point_container_t container;
    // https://mathworld.wolfram.com/Circle-LineIntersection.html
    int x1 = a.p1_x - b.center_x;
    int y1 = a.p1_y - b.center_y;
    int x2 = a.p2_x - b.center_x;
    int y2 = a.p2_y - b.center_y;
    int dx = x2 - x1;
    int dy = y2 - y1;
    ll dr2 = (ll) dx * dx + (ll) dy * dy;
    ll D = (ll) x1 * y2 - (ll) x2 * y1;
    double D2 = (double) D * D;
    double delta = (double) b.radius * b.radius * dr2 - D2;
    Coordinate sqrt_delta_L(sqrt(dr2) * b.radius);
    Coordinate sqrt_delta_R((double) abs(D));
    if (sqrt_delta_L < sqrt_delta_R) {
        std::get<0>(container) = true;
        return container;
    } else if (sqrt_delta_L == sqrt_delta_R) {
        Coordinate x(+(double) D / (double) dr2 * dy + b.center_x);
        Coordinate y(-(double) D / (double) dr2 * dx + b.center_y);
        if (a.checkPoint(x, y)) {
            _pushPoint(container, x, y);
        }
        std::get<0>(container) = true;
        return container;
    } else {
        double coeff_xx = dx * ((dy >= 0) ? 1 : -1);
        double coeff_yy = abs(dy);

        Coordinate xc1(+(double) D / (double) dr2 * dy + b.center_x + coeff_xx * sqrt(delta) / (double) dr2);
        Coordinate yc1(-(double) D / (double) dr2 * dx + b.center_y + coeff_yy * sqrt(delta) / (double) dr2);
        if (a.checkPoint(xc1, yc1)) {
            _pushPoint(container, xc1, yc1);
        }

        Coordinate xc2(+(double) D / (double) dr2 * dy + b.center_x - coeff_xx * sqrt(delta) / (double) dr2);
        Coordinate yc2(-(double) D / (double) dr2 * dx + b.center_y - coeff_yy * sqrt(delta) / (double) dr2);
        if (a.checkPoint(xc2, yc2)) {
            _pushPoint(container, xc2, yc2);
        }
        std::get<0>(container) = true;
        return container;
    }
}

point_container_t intersection(const Circle &a, const Line &b) {
//    std::cout << "Circle - Line!\n";
    return intersection(b, a);
}

inline ll dot(int sx1, int sy1, int tx1, int ty1, int sx2, int sy2, int tx2, int ty2) {
    ll dx1 = tx1 - sx1;
    ll dy1 = ty1 - sy1;
    ll dx2 = tx2 - sx2;
    ll dy2 = ty2 - sy2;
    return dx1 * dx2 + dy1 * dy2;
}

inline ll cross(int sx1, int sy1, int tx1, int ty1, int sx2, int sy2, int tx2, int ty2) {
    ll dx1 = tx1 - sx1;
    ll dy1 = ty1 - sy1;
    ll dx2 = tx2 - sx2;
    ll dy2 = ty2 - sy2;
    return dx1 * dy2 - dx2 * dy1;
}

inline bool
_checkHalfLineOverlap(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, point_container_t &container) {
    if (dot(x1, y1, x2, y2, x3, y3, x4, y4) < 0) {
        // different direction

        ll direction = dot(x1, y1, x2, y2, x1, y1, x3, y3); // (v1, v2) \dot (v1, v3)
        if (direction > 0) {
            // overlap
            return false;
        } else if (direction < 0) {
            return true;
        } else {
            // common endpoint
            // have a intersection
            Coordinate x_in((double) x1);
            Coordinate y_in((double) y1);
            _pushPoint(container, x_in, y_in);
            return true;
        }
    } else {
        return false;
    }
}

point_container_t intersection(const Line &a, const Line &b) {
    point_container_t container;
    // https://en.wikipedia.org/wiki/Line–line_intersection
//    std::cout << "Line - Line!\n";
    int x1 = a.p1_x, y1 = a.p1_y;
    int x2 = a.p2_x, y2 = a.p2_y;
    int x3 = b.p1_x, y3 = b.p1_y;
    int x4 = b.p2_x, y4 = b.p2_y;
    ll bottomL = (ll) (x1 - x2) * (y3 - y4);
    ll bottomR = (ll) (y1 - y2) * (x3 - x4);
    double bottom = (double) bottomL - (double) bottomR;

    if (bottomL == bottomR) {  // lines parallel
        int xtmp3 = x3;
        int xtmp4 = x4;
        int ytmp3 = y3;
        int ytmp4 = y4;
        if (dot(x1, y1, x2, y2, x3, y3, x4, y4) < 0) {  // line1 "-->", line2 "<--", line_temp "-->"
            std::swap(xtmp3, xtmp4);
            std::swap(ytmp3, ytmp4);
        }
        if (cross(x1, y1, xtmp4, ytmp4, x2, y2, xtmp3, ytmp3) == 0) {  // on the same line
            if (a.type == LineType::LINE || b.type == LineType::LINE) {
                std::get<0>(container) = false;
                return container;
            } else if (a.type == HALF_LINE && b.type == HALF_LINE) {
                bool res = _checkHalfLineOverlap(x1, y1, x2, y2, x3, y3, x4, y4, container);
                std::get<0>(container) = res;
                return container;
            } else if (a.type == SEGMENT_LINE && b.type == SEGMENT_LINE) {
                bool res = _checkHalfLineOverlap(x1, y1, x2, y2, x3, y3, x4, y4, container) ||
                           _checkHalfLineOverlap(x2, y2, x1, y1, x3, y3, x4, y4, container) ||
                           _checkHalfLineOverlap(x1, y1, x2, y2, x4, y4, x3, y3, container) ||
                           _checkHalfLineOverlap(x2, y2, x1, y1, x4, y4, x3, y3, container);
                std::get<0>(container) = res;
                return container;
            } else if (a.type == SEGMENT_LINE && b.type == HALF_LINE) {
                bool res = _checkHalfLineOverlap(x1, y1, x2, y2, x3, y3, x4, y4, container) ||
                           _checkHalfLineOverlap(x2, y2, x1, y1, x3, y3, x4, y4, container);
                std::get<0>(container) = res;
                return container;
            } else { // a.type == HALF_LINE && b.type == SEGMENT_LINE
                bool res = _checkHalfLineOverlap(x1, y1, x2, y2, x3, y3, x4, y4, container) ||
                           _checkHalfLineOverlap(x1, y1, x2, y2, x4, y4, x3, y3, container);
                std::get<0>(container) = res;
                return container;
            }
        }
        // parallel, but not on the same line
        std::get<0>(container) = true;
        return container;
    } else {
        // not parallel, bottom != 0, have one intersection point
        Coordinate xx(
                ((double) x1 * y2 / bottom - (double) y1 * x2 / bottom) * (x3 - x4) -
                ((double) x3 * y4 / bottom - (double) y3 * x4 / bottom) * (x1 - x2));
        Coordinate yy(
                ((double) x1 * y2 / bottom - (double) y1 * x2 / bottom) * (y3 - y4) -
                ((double) x3 * y4 / bottom - (double) y3 * x4 / bottom) * (y1 - y2));
        if (a.checkPoint(xx, yy) && b.checkPoint(xx, yy)) {
            _pushPoint(container, xx, yy);
        }
        std::get<0>(container) = true;
        return container;
    }
}

point_container_t intersection(const Circle &a, const Circle &b) {
    point_container_t container;
    // https://stackoverflow.com/questions/3349125/circle-circle-intersection-points
//    std::cout << "Circle - Circle!\n";
    int x1 = a.center_x, y1 = a.center_y, r1 = a.radius;
    int x2 = b.center_x, y2 = b.center_y, r2 = b.radius;

    if (x1 == x2 && y1 == y2 && r1 == r2) {
        std::get<0>(container) = false;
        return container;
    }

    ll d2 = square(x1 - x2) + square(y1 - y2);
    if (d2 > square(r1 + r2) || d2 < square(r1 - r2)) {
        std::get<0>(container) = true;
        return container;
    }

    double btm = 2.0 * d2;
    double add_x3 = (double) (square(r1) - square(r2) + d2) / btm * (x2 - x1) + x1;
    double add_y3 = (double) (square(r1) - square(r2) + d2) / btm * (y2 - y1) + y1;

    if (d2 == square(r1 + r2) || d2 == square(r1 - r2)) {
        // x1 + (square(r1)-square(r2)+d2)/(2*d2) * (x2-x1);
        // y1 + (square(r1)-square(r2)+d2)/(2*d2) * (y2-y1);
        Coordinate x_in(add_x3);
        Coordinate y_in(add_y3);
        _pushPoint(container, x_in, y_in);
        std::get<0>(container) = true;
        return container;
    } else {
        double coeff_x3 = (y2 - y1) / btm;
        double coeff_y3 = (x2 - x1) / btm;
        double insqrt = 4.0 * square(r1) * d2 - square(square(r1) - square(r2) + d2);
        Coordinate x_in1(add_x3 + coeff_x3 * sqrt(insqrt));
        Coordinate y_in1(add_y3 - coeff_y3 * sqrt(insqrt));
        Coordinate x_in2(add_x3 - coeff_x3 * sqrt(insqrt));
        Coordinate y_in2(add_y3 + coeff_y3 * sqrt(insqrt));
        _pushPoint(container, x_in1, y_in1);
        _pushPoint(container, x_in2, y_in2);
        std::get<0>(container) = true;
        return container;
    }
}

ll square(ll x) {
    return x * x;
}

bool checkPointHalf(const Coordinate &x, const Coordinate &y,
                    const Line &line) {
    Coordinate xc1(line.p1_x);
    Coordinate yc1(line.p1_y);
    Coordinate xc2(line.p2_x);
    Coordinate yc2(line.p2_y);

    bool x_check = false, y_check = false;

    if ((x < xc1 || x == xc1) && (xc2 < xc1 || xc2 == xc1)) {
        x_check = true;
    }
    if ((x > xc1 || x == xc1) && (xc2 > xc1 || xc2 == xc1)) {
        x_check = true;
    }
    if ((y < yc1 || y == yc1) && (yc2 < yc1 || yc2 == yc1)) {
        y_check = true;
    }
    if ((y > yc1 || y == yc1) && (yc2 > yc1 || yc2 == yc1)) {
        y_check = true;
    }
    return x_check & y_check;
}

bool checkPointSegment(const Coordinate &x, const Coordinate &y,
                       const Line &line) {
    Coordinate xc1(line.p1_x);
    Coordinate yc1(line.p1_y);
    Coordinate xc2(line.p2_x);
    Coordinate yc2(line.p2_y);

    bool x_check = false, y_check = false;

    if ((x < xc1 || x == xc1) && (xc2 < x || xc2 == x)) {
        x_check = true;
    }
    if ((x > xc1 || x == xc1) && (xc2 > x || xc2 == x)) {
        x_check = true;
    }
    if ((y < yc1 || y == yc1) && (yc2 < y || yc2 == y)) {
        y_check = true;
    }
    if ((y > yc1 || y == yc1) && (yc2 > y || yc2 == y)) {
        y_check = true;
    }
    return x_check & y_check;
}

gShape gShapeConverter(const Line &s) {
    if (s.type == LineType::LINE)
        return gShape{'L', s.p1_x, s.p1_y, s.p2_x, s.p2_y};
    else if (s.type == LineType::HALF_LINE)
        return gShape{'R', s.p1_x, s.p1_y, s.p2_x, s.p2_y};
    else
        return gShape{'S', s.p1_x, s.p1_y, s.p2_x, s.p2_y};
}

gShape gShapeConverter(const Circle &s) {
    return gShape{'C', s.center_x, s.center_y, s.radius, -1};
}
