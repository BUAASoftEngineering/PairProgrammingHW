//
// Created by Mist on 2020/3/18 0018.
//

#ifndef GEOMETRY_INTERFACE_H
#define GEOMETRY_INTERFACE_H

#include <unordered_set>
#include "Shapes.h"


enum ERROR_CODE {
    SUCCESS, INVALID_SHAPE, INTERSECTION_EXCP, INVALID_INPUT,
};

struct gManager {
    std::vector<Geometry> *shapes;
    std::unordered_set<Point, hashCode_Point, equals_Point> *points;
};


gManager *createManager() {
    auto *inst = new gManager;
    inst->shapes = new std::vector<Geometry>();
    inst->points = new std::unordered_set<Point, hashCode_Point, equals_Point>();
    return inst;
}

void closeManager(gManager *inst) {
    delete inst->shapes;
    delete inst->points;
    delete inst;
}

void cleanManager(gManager *inst) {
    inst->shapes->clear();
    inst->points->clear();
}


void _pushPoint(double *buf, const Point &point, int &pos) {
    auto &x = point.x;
    auto &y = point.y;
    double xVal, yVal;
    if (x.isRational) {
        xVal = (double) x.top / x.bottom;
    } else {
        xVal = x.value;
    }
    if (y.isRational) {
        yVal = (double) y.top / y.bottom;
    } else {
        yVal = y.value;
    }
    buf[pos++] = xVal;
    buf[pos++] = yVal;
}

/**
 *
 * @param inst
 * @param objType
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param buf nullable, buffer
 * @param posBuf
 * @return
 */
ERROR_CODE addShape(gManager *inst, char objType, int x1, int y1, int x2, int y2,
                    double *buf, int *posBuf) {
    Geometry obj = Line();

    // TODO: ERROR_CODE::INVALID_SHAPE (objType, x1, y1, x2, y2 check!!!)
    if (objType == 'L') {
        obj = Line(x1, y1, x2, y2);
    } else if (objType == 'C') {
        obj = Circle(x1, y1, x2);
    } else if (objType == 'R') {
        obj = Line(x1, y1, x2, y2, LineType::HALF_LINE);
    } else if (objType == 'S') {
        obj = Line(x1, y1, x2, y2, LineType::SEGMENT_LINE);
    } else {
        return ERROR_CODE::INVALID_SHAPE;
    }

    for (auto &objExist : *inst->shapes) {
        // TODO: ERROR_CODE::INTERSECTION_EXCP (parallel / overlap lines check!!!)
        std::vector<Point> intersections =
                std::visit(interset_visitor{}, obj, objExist);
        for (auto &p: intersections) {
            if (buf && inst->points->count(p) == 0) {
                _pushPoint(buf, p, *posBuf);  // increment --> points already exist shouldn't be returned
            }
            inst->points->insert(p);
        }
    }
    inst->shapes->push_back(obj);
    return ERROR_CODE::SUCCESS;
}

/**
 *
 * @param inst
 * @param inputFile nullable. if null, read from stdin
 * @param buf
 * @param posBuf
 * @return
 */
ERROR_CODE addShapesBatch(gManager *inst, FILE *inputFile, double *buf, int *posBuf) {
    // TODO: ERROR_CODE::INVALID_INPUT check (encapsulate a "TRY_READ_LINE" function for stdin/filein/GUIin !!!)
    int objCount;
    if (inputFile) {
        fscanf(inputFile, "%d", &objCount);
    } else {
        scanf("%d", &objCount);
    }

    // temp vars for input
    char objType;
    int x1, y1, x2, y2;

    for (int i = 0; i < objCount; ++i) {
        char str[5];
        if (inputFile) {
            fscanf(inputFile, "%s", str);
        } else {
            scanf("%s", str);
        }
        objType = str[0];
        if (objType == 'C') {
            if (inputFile) {
                fscanf(inputFile, "%d%d%d", &x1, &y1, &x2);
            } else {
                scanf("%d%d%d", &x1, &y1, &x2);
            }
            y2 = -1;
        } else {
            if (inputFile) {
                fscanf(inputFile, "%d%d%d%d", &x1, &y1, &x2, &y2);
            } else {
                scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
            }
        }
        ERROR_CODE status = addShape(inst, objType, x1, y1, x2, y2, buf, posBuf);
        if (status != ERROR_CODE::SUCCESS)
            return status;
    }
    return ERROR_CODE::SUCCESS;
}


int getIntersectionsCount(gManager *inst) {
    return inst->points->size();
}

void getIntersections(gManager *inst, double *buf) {
    int pos = 0;
    for (auto &point: *inst->points) {
        _pushPoint(buf, point, pos);
    }
}

#endif //GEOMETRY_INTERFACE_H
