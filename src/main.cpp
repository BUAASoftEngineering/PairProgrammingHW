#include <iostream>
#include <cstring>
#include "StdInterface.h"

int main(int argc, char *argv[]) {
    // handle arguments & freopen
    char *filein = nullptr;
    FILE *fileout = nullptr;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0) {
            filein = argv[i + 1];
        }
        if (strcmp(argv[i], "-o") == 0) {
            fileout = fopen(argv[i + 1], "w");
        }
    }

    // create Figure
    auto *fig = addFigure();

    // add Geometry Shapes From File
    ERROR_INFO errInfo;
    if (filein)
        errInfo = addShapesToFigureFile(fig, filein);
    else
        errInfo = addShapesToFigureStdin(fig);
    if (errInfo.code != ERROR_CODE::SUCCESS) {
        std::string errMsg;
        switch (errInfo.code) {
            case ERROR_CODE::WRONG_FORMAT:
                errMsg = "Wrong Format";
                break;
            case ERROR_CODE::VALUE_OUT_OF_RANGE:
                errMsg = "Value Out of Range";
                break;
            case ERROR_CODE::INVALID_LINE:
                errMsg = "Invalid Line";
                break;
            case ERROR_CODE::INVALID_CIRCLE:
                errMsg = "Invalid Circle";
                break;
            case ERROR_CODE::LINE_OVERLAP:
                errMsg = "Line Overlap";
                break;
            case ERROR_CODE::CIRCLE_OVERLAP:
                errMsg = "Circle Overlap";
                break;
            default:
                errMsg = "Unknown Error";
        }
        std::cout << "ERROR: " << errMsg;
        if (errInfo.lineNoStartedWithZero != -1)
            std::cout << " with line no." << errInfo.lineNoStartedWithZero + 1;
        if (strcmp(errInfo.messages, "") != 0)
            std::cout << " ::\n    " << errInfo.messages << std::endl;
        else
            std::cout << std::endl;
    }

    int intersectionsCount = getPointsCount(fig);
    if (fileout) {
        fprintf(fileout, "%d\n", intersectionsCount);
    } else {
        printf("%d\n", intersectionsCount);
    }
//    printf("hash:%d, equals:%d\n", GLOBAL_HASH_COUNT, GLOBAL_COLLISION_COUNT);
    /*
    updatePoints(fig);
    for (int i = 0; i < intersectionsCount; ++i) {
        if (fileout)
            fprintf(fileout, "%.3lf,%.3lf\n", fig->points[i].x, fig->points[i].y);
        else
            printf("%.3lf,%.3lf\n", fig->points[i].x, fig->points[i].y);
    }
    */
    // close Figure
//    deleteFigure(fig);
    return 0;
}
