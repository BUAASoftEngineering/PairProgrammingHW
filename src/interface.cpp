//
// Created by Mist on 2020/3/20 0020.
//

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
