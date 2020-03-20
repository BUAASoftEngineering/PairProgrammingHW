//
// Created by Mist on 2020/3/20 0020.
//

#include <cstring>
#include "interface.h"

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
    buf[pos++] = point.x.value;
    buf[pos++] = point.y.value;
}

ERROR_CODE addShape(gManager *inst, char objType, int x1, int y1, int x2, int y2, double *buf, int *posBuf) {
    Geometry obj = Line();

    // TODO: ERROR_CODE::INVALID_SHAPE (objType, x1, y1, x2, y2 check!!!)

    if (abs(x1) >= 1e5 || abs(y1) >= 1e5 || abs(x2) >= 1e5 || abs(y2) >= 1e5) {
        return ERROR_CODE::INVALID_SHAPE;
    }

    if (objType == 'L') {
        if (x1 == x2 && y1 == y2) {
            return ERROR_CODE::INVALID_SHAPE;
        }
        obj = Line(x1, y1, x2, y2);
    } else if (objType == 'C') {
        if (x2 <= 0) {
            return ERROR_CODE::INVALID_SHAPE;
        }
        obj = Circle(x1, y1, x2);
    } else if (objType == 'R') {
        if (x1 == x2 && y1 == y2) {
            return ERROR_CODE::INVALID_SHAPE;
        }
        obj = Line(x1, y1, x2, y2, LineType::HALF_LINE);
    } else if (objType == 'S') {
        if (x1 == x2 && y1 == y2) {
            return ERROR_CODE::INVALID_SHAPE;
        }
        obj = Line(x1, y1, x2, y2, LineType::SEGMENT_LINE);
    } else {
        return ERROR_CODE::INVALID_SHAPE;
    }

    for (auto &objExist : *inst->shapes) {
        point_container_t intersections =
                std::visit(interset_visitor{}, obj, objExist);

        bool isLegal;
        int size;
        Point p1{}, p2{};
        std::tie(isLegal, size, p1, p2) = intersections;

        if (!isLegal) {
            // overlap lines
            return ERROR_CODE::INTERSECTION_EXCP;
        }
        if (size >= 1) {
            if (buf && inst->points->count(p1) == 0)
                _pushPoint(buf, p1, *posBuf);  // increment --> points already exist shouldn't be returned
            inst->points->insert(p1);
        }
        if (size >= 2) {
            if (buf && inst->points->count(p2) == 0)
                _pushPoint(buf, p2, *posBuf);  // increment --> points already exist shouldn't be returned
            inst->points->insert(p2);
        }
    }
    inst->shapes->push_back(obj);
    return ERROR_CODE::SUCCESS;
}

int readWordToBuffer(FILE *inputFile, char *buf) {
    if (inputFile) {
        return fscanf(inputFile, "%s", buf);
    } else {
        return scanf("%s", buf);
    }
}

ERROR_CODE readInt(FILE *inputFile, int &dst) {
    char buf[256];
    char *stop;
    int r = readWordToBuffer(inputFile, buf);
    if (r == EOF) { return ERROR_CODE::INVALID_INPUT; }
    if (strlen(buf) > 10) { return ERROR_CODE::INVALID_INPUT; }
    int res = strtol(buf, &stop, 0);
    if (*stop) { return ERROR_CODE::INVALID_INPUT; }
    dst = res;
    return ERROR_CODE::SUCCESS;
}

ERROR_CODE readChar(FILE *inputFile, char &dst) {
    char buf[256];
    int r = readWordToBuffer(inputFile, buf);
    if (r == EOF) { return ERROR_CODE::INVALID_INPUT; }
    if (strlen(buf) != 1) { return ERROR_CODE::INVALID_INPUT; }
    dst = buf[0];
    return ERROR_CODE::SUCCESS;
}

ERROR_CODE addShapesBatch(gManager *inst, FILE *inputFile, double *buf, int *posBuf) {
    // TODO: ERROR_CODE::INVALID_INPUT check (encapsulate a "TRY_READ_LINE" function for stdin/filein/GUIin !!!)
    // TODO: finer classification of ERROR_CODE::INVALID_INPUT
    int objCount;
    ERROR_CODE status;
    status = readInt(inputFile, objCount);
    if (status != ERROR_CODE::SUCCESS) { return status; }
    if (objCount <= 0) { return ERROR_CODE::INVALID_INPUT; }

    // temp vars for input
    char objType;
    int x1, y1, x2, y2;

    for (int i = 0; i < objCount; ++i) {
        status = readChar(inputFile, objType);
        if (status != ERROR_CODE::SUCCESS) { return status; }

        status = readInt(inputFile, x1);
        if (status != ERROR_CODE::SUCCESS) { return status; }
        status = readInt(inputFile, y1);
        if (status != ERROR_CODE::SUCCESS) { return status; }
        status = readInt(inputFile, x2);
        if (status != ERROR_CODE::SUCCESS) { return status; }

        if (objType == 'C') {
            y2 = -1;
        } else {
            status = readInt(inputFile, y2);
            if (status != ERROR_CODE::SUCCESS) { return status; }
        }

        status = addShape(inst, objType, x1, y1, x2, y2, buf, posBuf);
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
