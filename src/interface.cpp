//
// Created by Mist on 2020/3/20 0020.
//

#include <cstring>
#include "interface.h"

gManager *createManager() {
    auto *inst = new gManager;
    inst->shapes = new std::vector<Geometry>();
    inst->points = new std::unordered_set<Point, hashCode_Point, equals_Point>();
    inst->upperleft = gPoint{-5, 5};
    inst->lowerright = gPoint{5, -5};
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
    inst->upperleft = gPoint{-5, 5};
    inst->lowerright = gPoint{5, -5};
}

inline void _pushPoint(gManager *inst, gPoint *buf, const Point &point, int &pos) {
    buf[pos++] = gPoint{point.x.value, point.y.value};
    inst->upperleft.x = std::min(inst->upperleft.x, point.x.value);
    inst->upperleft.y = std::max(inst->upperleft.y, point.y.value);
    inst->lowerright.x = std::max(inst->lowerright.x, point.x.value);
    inst->lowerright.y = std::min(inst->lowerright.y, point.y.value);
}

ERROR_INFO addShape(gManager *inst, char objType, int x1, int y1, int x2, int y2, gPoint *buf, int *posBuf) {
    Geometry obj = Line();

    if (abs(x1) >= 1e5 || abs(y1) >= 1e5 || abs(x2) >= 1e5 || abs(y2) >= 1e5) {
        return ERROR_INFO{ERROR_CODE::VALUE_OUT_OF_RANGE, -1, "Parameters should be within (-100000, 100000) !"};
    }

    if (objType == 'L') {
        if (x1 == x2 && y1 == y2) {
            return ERROR_INFO{ERROR_CODE::INVALID_LINE, -1, "Needs two points to define a line !"};
        }
        obj = Line(x1, y1, x2, y2);
    } else if (objType == 'C') {
        if (x2 <= 0) {
            return ERROR_INFO{ERROR_CODE::INVALID_CIRCLE, -1, "The radius of a circle should be positive !"};
        }
        obj = Circle(x1, y1, x2);
    } else if (objType == 'R') {
        if (x1 == x2 && y1 == y2) {
            return ERROR_INFO{ERROR_CODE::INVALID_LINE, -1, "Needs two points to define a halfline !"};
        }
        obj = Line(x1, y1, x2, y2, LineType::HALF_LINE);
    } else if (objType == 'S') {
        if (x1 == x2 && y1 == y2) {
            return ERROR_INFO{ERROR_CODE::INVALID_LINE, -1, "Needs two points to define a segment !"};
        }
        obj = Line(x1, y1, x2, y2, LineType::SEGMENT_LINE);
    } else {
        return ERROR_INFO{ERROR_CODE::WRONG_FORMAT, -1,
                          "The type should be 'C', 'L', 'R' or 'S' !"};
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
            if (objType == 'C')
                return ERROR_INFO{ERROR_CODE::CIRCLE_OVERLAP, -1, "This circle overlaps with another circle !"};
            else
                return ERROR_INFO{ERROR_CODE::LINE_OVERLAP, -1, "This line overlaps with another line !"};
        }
        if (size >= 1) {
            if (buf && inst->points->count(p1) == 0)
                _pushPoint(inst, buf, p1, *posBuf);  // increment --> points already exist shouldn't be returned
            inst->points->insert(p1);
        }
        if (size >= 2) {
            if (buf && inst->points->count(p2) == 0)
                _pushPoint(inst, buf, p2, *posBuf);  // increment --> points already exist shouldn't be returned
            inst->points->insert(p2);
        }
    }
    inst->shapes->push_back(obj);
    return ERROR_INFO{};
}

inline int readWordToBuffer(FILE *inputFile, char *buf) {
    if (inputFile) {
        return fscanf(inputFile, "%s", buf);
    } else {
        return scanf("%s", buf);
    }
}

inline ERROR_CODE readInt(FILE *inputFile, int &dst) {
    char buf[256];
    char *stop;
    int r = readWordToBuffer(inputFile, buf);
    if (r == EOF) { return ERROR_CODE::WRONG_FORMAT; }
    if (strlen(buf) > 10) { return ERROR_CODE::WRONG_FORMAT; }
    int res = strtol(buf, &stop, 10);
    if (*stop) { return ERROR_CODE::WRONG_FORMAT; }
    dst = res;
    return ERROR_CODE::SUCCESS;
}

inline ERROR_CODE readChar(FILE *inputFile, char &dst) {
    char buf[256];
    int r = readWordToBuffer(inputFile, buf);
    if (r == EOF) { return ERROR_CODE::WRONG_FORMAT; }
    if (strlen(buf) != 1) { return ERROR_CODE::WRONG_FORMAT; }
    dst = buf[0];
    return ERROR_CODE::SUCCESS;
}

ERROR_INFO addShapesBatch(gManager *inst, FILE *inputFile, gPoint *buf, int *posBuf) {
    int objCount;
    ERROR_CODE status;
    status = readInt(inputFile, objCount);
    if (status != ERROR_CODE::SUCCESS) {
        return ERROR_INFO{status, 0, "#shapes should be an integer !"};
    }
    if (objCount <= 0) {
        return ERROR_INFO{ERROR_CODE::WRONG_FORMAT, 0, "#shapes should > 0 !"};
    }

    // RESERVE SPACE FOR POINTS with magic number (max_load_factor) 0.75
    inst->points->rehash(int(objCount * objCount / 2.0 / 0.75) + 1);

    // temp vars for input
    char objType;
    int x1, y1, x2, y2;

    for (int i = 0; i < objCount; ++i) {
        status = readChar(inputFile, objType);
        if (status != ERROR_CODE::SUCCESS) {
            return ERROR_INFO{status, i + 1, "The type of shape should be one character !"};
        }

        status = readInt(inputFile, x1);
        if (status != ERROR_CODE::SUCCESS) {
            return ERROR_INFO{status, i + 1, "The args of shape should be an integer !"};
        }
        status = readInt(inputFile, y1);
        if (status != ERROR_CODE::SUCCESS) {
            return ERROR_INFO{status, i + 1, "The args of shape should be an integer !"};
        }
        status = readInt(inputFile, x2);
        if (status != ERROR_CODE::SUCCESS) {
            return ERROR_INFO{status, i + 1, "The args of shape should be an integer !"};
        }

        if (objType == 'C') {
            y2 = -1;
        } else {
            status = readInt(inputFile, y2);
            if (status != ERROR_CODE::SUCCESS) {
                return ERROR_INFO{status, i + 1, "The args of shape should be an integer !"};
            }
        }

        ERROR_INFO singleShape = addShape(inst, objType, x1, y1, x2, y2, buf, posBuf);
        if (singleShape.code != ERROR_CODE::SUCCESS) {
            singleShape.lineNoStartedWithZero = i + 1;
            return singleShape;
        }
    }
    return ERROR_INFO{};
}

int getIntersectionsCount(gManager *inst) {
    return inst->points->size();
}

int getGeometricShapesCount(gManager *inst) {
    return inst->shapes->size();
}

void getIntersections(gManager *inst, gPoint *buf) {
    int pos = 0;
    for (auto &point: *inst->points) {
        _pushPoint(inst, buf, point, pos);
    }
}

void getGeometricShapes(gManager *inst, gShape *buf) {
    int pos = 0;
    for (auto &shape: *inst->shapes) {
        buf[pos++] = std::visit(gShape_visitor{}, shape);
    }
}
