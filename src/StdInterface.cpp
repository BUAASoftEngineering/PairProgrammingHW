//
// Created by xiangweilai on 2020/3/22.
//

#include "StdInterface.h"
#include "interface.h"

std::vector<gManager *> gManagers;

gFigure *addFigure() {
    auto mng = createManager();
    gManagers.push_back(mng);
    auto fig = new gFigure;
    fig->figureId = gManagers.size() - 1;
    fig->shapes = nullptr;
    fig->points = nullptr;
    fig->upperleft = {-5, 5};
    fig->lowerright = {5, -5};
    return fig;
}

void deleteFigure(gFigure *fig) {
    auto mng = gManagers[fig->figureId];
    closeManager(mng);
    delete fig->shapes;
    delete fig->points;
    gManagers.erase(gManagers.begin() + fig->figureId);
    delete fig;
}

void cleanFigure(gFigure *fig) {
    auto mng = gManagers[fig->figureId];
    delete fig->shapes;
    fig->shapes = nullptr;
    delete fig->points;
    fig->points = nullptr;
    fig->upperleft = gPoint{-5, 5};
    fig->lowerright = gPoint{5, -5};
    cleanManager(mng);
}


inline int readWordToBuffer(char *&inputString, char *buf) {
    int pos = 0;
    auto result = sscanf(inputString, "%s%n", buf, &pos);
    if (result == 1)
        inputString += pos;
    return result;
}

inline ERROR_CODE readInt(char *&inputString, int &dst) {
    char buf[256];
    char *stop;
    int r = readWordToBuffer(inputString, buf);
    if (r == EOF) { return ERROR_CODE::WRONG_FORMAT; }
    if (strlen(buf) > 10) { return ERROR_CODE::WRONG_FORMAT; }
    int res = strtol(buf, &stop, 10);
    if (*stop) { return ERROR_CODE::WRONG_FORMAT; }
    dst = res;
    return ERROR_CODE::SUCCESS;
}

inline ERROR_CODE readChar(char *&inputString, char &dst) {
    char buf[256];
    int r = readWordToBuffer(inputString, buf);
    if (r == EOF) { return ERROR_CODE::WRONG_FORMAT; }
    if (strlen(buf) != 1) { return ERROR_CODE::WRONG_FORMAT; }
    dst = buf[0];
    return ERROR_CODE::SUCCESS;
}


ERROR_INFO addShapeToFigure(gFigure *fig, gShape obj) {
    auto mng = gManagers[fig->figureId];
    auto ret = addShape(mng, obj.type, obj.x1, obj.y1, obj.x2, obj.y2, nullptr, nullptr);
    fig->upperleft = mng->upperleft;
    fig->lowerright = mng->lowerright;
    return ret;
}

ERROR_INFO addShapeToFigureString(gFigure *fig, const char *desc) {
    int length = strlen(desc);
    if (length > 1000)
        return ERROR_INFO{ERROR_CODE::WRONG_FORMAT, -1, "String Length Out Of Range 1000 !"};
    char desc_copy[1000];
    char *desc_copy_ptr = desc_copy;
    strcpy(desc_copy, desc);

    // temp vars for input
    char objType;
    int x1, y1, x2, y2;
    ERROR_CODE status;

    status = readChar(desc_copy_ptr, objType);
    if (status != ERROR_CODE::SUCCESS) {
        return ERROR_INFO{status, -1, "The type of shape should be one character !"};
    }

    status = readInt(desc_copy_ptr, x1);
    if (status != ERROR_CODE::SUCCESS) {
        return ERROR_INFO{status, -1, "The args of shape should be an integer !"};
    }
    status = readInt(desc_copy_ptr, y1);
    if (status != ERROR_CODE::SUCCESS) {
        return ERROR_INFO{status, -1, "The args of shape should be an integer !"};
    }
    status = readInt(desc_copy_ptr, x2);
    if (status != ERROR_CODE::SUCCESS) {
        return ERROR_INFO{status, -1, "The args of shape should be an integer !"};
    }

    if (objType == 'C') {
        y2 = -1;
    } else {
        status = readInt(desc_copy_ptr, y2);
        if (status != ERROR_CODE::SUCCESS) {
            return ERROR_INFO{status, -1, "The args of shape should be an integer !"};
        }
    }

    auto mng = gManagers[fig->figureId];
    auto ret = addShape(mng, objType, x1, y1, x2, y2, nullptr, nullptr);
    fig->upperleft = mng->upperleft;
    fig->lowerright = mng->lowerright;
    return ret;
}

ERROR_INFO addShapesToFigureFile(gFigure *fig, const char *filename) {
    auto mng = gManagers[fig->figureId];
    FILE *filein = fopen(filename, "r");
    auto ret = addShapesBatch(mng, filein, nullptr, nullptr);
    fig->upperleft = mng->upperleft;
    fig->lowerright = mng->lowerright;
    return ret;
}

ERROR_INFO addShapesToFigureStdin(gFigure *fig) {
    auto mng = gManagers[fig->figureId];
    auto ret = addShapesBatch(mng, nullptr, nullptr, nullptr);
    fig->upperleft = mng->upperleft;
    fig->lowerright = mng->lowerright;
    return ret;
}

void removeShapeByIndex(gFigure *fig, unsigned int index) {
    auto mng = gManagers[fig->figureId];

    // backup shapes memory, remove obj at index
    auto objCount_prev = getGeometricShapesCount(mng);
    auto *shapes_mem = new gShape[objCount_prev];
    getGeometricShapes(mng, shapes_mem);

    // clean up
    cleanManager(mng);

    // reconstruct, without ERRORs
    mng->points->rehash(int(objCount_prev * objCount_prev / 2.0 / 0.75) + 1);

    for (int i = 0; i < objCount_prev; ++i) {
        if (i == index)
            continue;
        auto thisShape = shapes_mem[i];
        addShape(mng, thisShape.type, thisShape.x1, thisShape.y1, thisShape.x2, thisShape.y2, nullptr, nullptr);
    }
    fig->upperleft = mng->upperleft;
    fig->lowerright = mng->lowerright;
    delete[] shapes_mem;
}

int getPointsCount(const gFigure *fig) {
    auto mng = gManagers[fig->figureId];
    return getIntersectionsCount(mng);
}

int getShapesCount(const gFigure *fig) {
    auto mng = gManagers[fig->figureId];
    return getGeometricShapesCount(mng);
}

void updatePoints(gFigure *fig) {
    auto mng = gManagers[fig->figureId];
    delete fig->points;
    fig->points = new gPoint[getIntersectionsCount(mng)];
    getIntersections(mng, fig->points);
}

void updateShapes(gFigure *fig) {
    auto mng = gManagers[fig->figureId];
    delete fig->shapes;
    fig->shapes = new gShape[getGeometricShapesCount(mng)];
    getGeometricShapes(mng, fig->shapes);
}
