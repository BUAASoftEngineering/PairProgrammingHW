//
// Created by Mist on 2020/3/18 0018.
//

#ifndef GEOMETRY_INTERFACE_H
#define GEOMETRY_INTERFACE_H

#include <unordered_set>
#include "Shapes.h"

enum ERROR_CODE {
    SUCCESS, ERR_INVALID_OBJECT, ERR_INVALID_OBJ_ARGUMENT
};

struct geo_mgr_t {
    std::vector<Geometry> *objs;
    std::unordered_set<Point, hashCode_Point, equals_Point> *point_set;
};

geo_mgr_t *init_geo_instance() {
    auto *inst = new geo_mgr_t;
    inst->objs = new std::vector<Geometry>();
    inst->point_set = new std::unordered_set<Point, hashCode_Point, equals_Point>();
    return inst;
}

void close_geo_instance(geo_mgr_t *inst) {
    delete inst->objs;
    delete inst->point_set;
    delete inst;
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
int add(geo_mgr_t *inst, char objType, int x1, int y1, int x2, int y2,
        double *buf, int *posBuf) {
    auto *objs = inst->objs;
    Geometry obj = Line(0, 0, 0, 0);
    if (objType == 'L') {
        obj = Line(x1, y1, x2, y2);
    } else if (objType == 'C') {
        obj = Circle(x1, y1, x2);
    } else if (objType == 'R') {
        obj = Line(x1, y1, x2, y2, LineType::HALF_LINE);
    } else if (objType == 'S') {
        obj = Line(x1, y1, x2, y2, LineType::SEGMENT_LINE);
    } else {
        return ERROR_CODE::ERR_INVALID_OBJ_ARGUMENT;
    }
    for (auto &objExist :*objs) {
        std::vector<Point> intersections =
                std::visit(interset_visitor{}, obj, objExist);
        for (Point p: intersections) {
            inst->point_set->insert(p);
            if (buf) {
                _pushPoint(buf, p, *posBuf);
            }
        }
    }
    inst->objs->push_back(obj);
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
int addBatchFromFile(geo_mgr_t *inst, FILE *inputFile, double *buf, int *posBuf) {
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
        int err = add(inst, objType, x1, y1, x2, y2, buf, posBuf);
        if (err != ERROR_CODE::SUCCESS) {
            return err;
        }
    }
    return ERROR_CODE::SUCCESS;
}

int getNumIntersection(geo_mgr_t *inst) {
    return inst->point_set->size();
}


void getIntersections(geo_mgr_t *inst, double *buf) {
    auto *pset = inst->point_set;
    int pos = 0;
    for (auto &point: *pset) {
        _pushPoint(buf, point, pos);
    }
}

#endif //GEOMETRY_INTERFACE_H
