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


gManager *createManager();

void closeManager(gManager *inst);

void cleanManager(gManager *inst);


void _pushPoint(double *buf, const Point &point, int &pos);

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
                    double *buf, int *posBuf);

/**
 *
 * @param inst
 * @param inputFile nullable. if null, read from stdin
 * @param buf
 * @param posBuf
 * @return
 */
ERROR_CODE addShapesBatch(gManager *inst, FILE *inputFile, double *buf, int *posBuf);


int getIntersectionsCount(gManager *inst);

void getIntersections(gManager *inst, double *buf);

#endif //GEOMETRY_INTERFACE_H
