//
// Created by Mist on 2020/3/18 0018.
//

#ifndef GEOMETRY_INTERFACE_H
#define GEOMETRY_INTERFACE_H

#include <unordered_set>
#include "Shapes.h"


enum ERROR_CODE {
    SUCCESS,
    WRONG_FORMAT,
    VALUE_OUT_OF_RANGE,
    INVALID_LINE, INVALID_CIRCLE,
    LINE_OVERLAP, CIRCLE_OVERLAP,
};

struct ERROR_INFO {
    ERROR_CODE code = SUCCESS;
    int lineNoStartedWithZero = -1;
    char messages[50] = "";
};

struct gManager {
    std::vector<Geometry> *shapes;
    std::unordered_set<Point, hashCode_Point, equals_Point> *points;
    gPoint upperleft;
    gPoint lowerright;
};


gManager *createManager();

void closeManager(gManager *inst);

void cleanManager(gManager *inst);


void _pushPoint(gManager *inst, gPoint *buf, const Point &point, int &pos);

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
ERROR_INFO addShape(gManager *inst, char objType, int x1, int y1, int x2, int y2,
                    gPoint *buf, int *posBuf);

/**
 *
 * @param inst
 * @param inputFile nullable. if null, read from stdin
 * @param buf
 * @param posBuf
 * @return
 */
ERROR_INFO addShapesBatch(gManager *inst, FILE *inputFile, gPoint *buf, int *posBuf);


int getIntersectionsCount(gManager *inst);

int getGeometricShapesCount(gManager *inst);

void getIntersections(gManager *inst, gPoint *buf);

void getGeometricShapes(gManager *inst, gShape *buf);

#endif //GEOMETRY_INTERFACE_H
