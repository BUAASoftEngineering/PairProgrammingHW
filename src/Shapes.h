//
// Created by xiangweilai on 2020/3/9.
//

#ifndef GEOMETRY_SHAPES_H
#define GEOMETRY_SHAPES_H

#include <variant>
#include <vector>
#include <iostream>
#include "Point.h"

enum LineType {
    LINE = 0,
    HALF_LINE = 1,
    SEGMENT_LINE = 2
};

class Line {
public:
    Line(int x1, int y1, int x2, int y2);

    Line(int x1, int y1, int x2, int y2, int type);

    int p1_x, p1_y;
    int p2_x, p2_y;
    int type;

    virtual bool checkPoint(const Coordinate &x, const Coordinate &y) const;
};

bool checkPointHalf(const Coordinate &x, const Coordinate &y,
                    const Line &line);

bool checkPointSegment(const Coordinate &x, const Coordinate &y,
                       const Line &line);

//class HalfLine: public Line {
//public:
//    bool checkPoint(Coordinate x, Coordinate y) const override ;
//
//    HalfLine(int x1, int y1, int x2, int y2);
//};
//
//class SegmentLine: public Line {
//public:
//    bool checkPoint(Coordinate x, Coordinate y) const override ;
//
//    SegmentLine(int x1, int y1, int x2, int y2);
//};



class Circle {
public:
    Circle(int x, int y, int r);

    int center_x, center_y;
    int radius;
};

std::vector<Point> intersection(Line x, Circle y);

std::vector<Point> intersection(Circle x, Line y);

std::vector<Point> intersection(Line x, Line y);

std::vector<Point> intersection(Circle x, Circle y);

ll square(ll x);

// variant for auto combination
using Geometry = std::variant<Line, Circle>;

struct interset_visitor {
    template<typename Shape1, typename Shape2>
    std::vector<Point> operator()(const Shape1 &lhs, const Shape2 &rhs) const {
        return intersection(lhs, rhs);
    }
};


#endif //GEOMETRY_SHAPES_H
