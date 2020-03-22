//
// Created by xiangweilai on 2020/3/22.
//

#ifndef GEOMETRY_STDINTERFACE_H
#define GEOMETRY_STDINTERFACE_H

#include "interface.h"
#include <vector>

std::vector<gManager *> gManagers;

struct gFigure {
    unsigned int figureId = 0;
    gShape *shapes;  // only available after updateShapes()
    gPoint *points;  // only available after updatePoints()
    gPoint upperleft;
    gPoint lowerright;
};

gFigure *addFigure();

void deleteFigure(gFigure *fig);

void cleanFigure(gFigure *fig);

ERROR_INFO addShapeToFigure(gFigure *fig, gShape obj);

ERROR_INFO addShapeToFigureString(gFigure *fig, char *desc);

ERROR_INFO addShapesToFigureFile(gFigure *fig, char *filename);

ERROR_INFO addShapesToFigureStdin(gFigure *fig);

void removeShapeByIndex(gFigure *fig, unsigned int index);

int getPointsCount(gFigure *fig);

int getShapesCount(gFigure *fig);

void updatePoints(gFigure *fig);

void updateShapes(gFigure *fig);

#endif //GEOMETRY_STDINTERFACE_H
