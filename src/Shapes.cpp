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


std::vector<Point> intersection(const Line &a, const Circle &b) {
//    std::cout << "Line - Circle!\n";
    std::vector<Point> container;
    // https://mathworld.wolfram.com/Circle-LineIntersection.html
    int x1 = a.p1_x - b.center_x;
    int y1 = a.p1_y - b.center_y;
    int x2 = a.p2_x - b.center_x;
    int y2 = a.p2_y - b.center_y;
    int dx = x2 - x1;
    int dy = y2 - y1;
    ll dr2 = (ll) dx * dx + (ll) dy * dy;
    ll D = (ll) x1 * y2 - (ll) x2 * y1;
    ll D2 = D * D;
    ll delta = (ll) b.radius * b.radius * dr2 - D2;
    if (delta < 0)
        return container;
    else if (delta == 0) {
        Coordinate x(+(double) D / (double) dr2 * dy + b.center_x);
        Coordinate y(-(double) D / (double) dr2 * dx + b.center_y);
        if (a.checkPoint(x, y)) {
            container.emplace_back(x, y);
        }
        return container;
    } else {
        double coeff_xx = dx * ((dy >= 0) ? 1 : -1);
        double coeff_yy = abs(dy);

        Coordinate xc1(+(double) D / (double) dr2 * dy + b.center_x + coeff_xx * sqrt(delta) / (double) dr2);
        Coordinate yc1(-(double) D / (double) dr2 * dx + b.center_y + coeff_yy * sqrt(delta) / (double) dr2);
        if (a.checkPoint(xc1, yc1)) {
            container.emplace_back(xc1, yc1);
        }

        Coordinate xc2(+(double) D / (double) dr2 * dy + b.center_x - coeff_xx * sqrt(delta) / (double) dr2);
        Coordinate yc2(-(double) D / (double) dr2 * dx + b.center_y - coeff_yy * sqrt(delta) / (double) dr2);
        if (a.checkPoint(xc2, yc2)) {
            container.emplace_back(xc2, yc2);
        }
        return container;
    }
}

std::vector<Point> intersection(const Circle &a, const Line &b) {
//    std::cout << "Circle - Line!\n";
    return intersection(b, a);
}

std::vector<Point> intersection(const Line &a, const Line &b) {
    std::vector<Point> container;
    // https://en.wikipedia.org/wiki/Line–line_intersection
//    std::cout << "Line - Line!\n";
    int x1 = a.p1_x, y1 = a.p1_y;
    int x2 = a.p2_x, y2 = a.p2_y;
    int x3 = b.p1_x, y3 = b.p1_y;
    int x4 = b.p2_x, y4 = b.p2_y;
    ll bottomL = (ll) (x1 - x2) * (y3 - y4);
    ll bottomR = (ll) (y1 - y2) * (x3 - x4);
    double bottom = (double) bottomL - (double) bottomR;

    if (bottomL == bottomR)  // bottom == 0, inf
        return container;
    else {                   // bottom != 0, not inf
        Coordinate xx(
                ((double) x1 * y2 / bottom - (double) y1 * x2 / bottom) * (x3 - x4) -
                ((double) x3 * y4 / bottom - (double) y3 * x4 / bottom) * (x1 - x2));
        Coordinate yy(
                ((double) x1 * y2 / bottom - (double) y1 * x2 / bottom) * (y3 - y4) -
                ((double) x3 * y4 / bottom - (double) y3 * x4 / bottom) * (y1 - y2));
        if (a.checkPoint(xx, yy) && b.checkPoint(xx, yy)) {
            container.emplace_back(xx, yy);
        }
        return container;
    }
}

std::vector<Point> intersection(const Circle &a, const Circle &b) {
    std::vector<Point> container;
    // https://stackoverflow.com/questions/3349125/circle-circle-intersection-points
//    std::cout << "Circle - Circle!\n";
    int x1 = a.center_x, y1 = a.center_y, r1 = a.radius;
    int x2 = b.center_x, y2 = b.center_y, r2 = b.radius;
    ll d2 = square(x1 - x2) + square(y1 - y2);
    if (d2 > square(r1 + r2) || d2 < square(r1 - r2))
        return container;

    double btm = 2.0 * d2;
    double add_x3 = (double) (square(r1) - square(r2) + d2) / btm * (x2 - x1) + x1;
    double add_y3 = (double) (square(r1) - square(r2) + d2) / btm * (y2 - y1) + y1;

    if (d2 == square(r1 + r2) || d2 == square(r1 - r2)) {
        // x1 + (square(r1)-square(r2)+d2)/(2*d2) * (x2-x1);
        // y1 + (square(r1)-square(r2)+d2)/(2*d2) * (y2-y1);
        container.emplace_back(Coordinate(add_x3), Coordinate(add_y3));
        return container;
    } else {
        double coeff_x3 = (y2 - y1) / btm;
        double coeff_y3 = (x2 - x1) / btm;
        double insqrt = 4.0 * square(r1) * d2 - square(square(r1) - square(r2) + d2);
        container.emplace_back(Coordinate(add_x3 + coeff_x3 * sqrt(insqrt)),
                               Coordinate(add_y3 - coeff_y3 * sqrt(insqrt)));
        container.emplace_back(Coordinate(add_x3 - coeff_x3 * sqrt(insqrt)),
                               Coordinate(add_y3 + coeff_y3 * sqrt(insqrt)));
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
