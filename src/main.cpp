#include <iostream>
#include <cstring>
#include "interface.h"

int main(int argc, char *argv[]) {
    // handle arguments & freopen
    FILE *filein = nullptr;
    FILE *fileout = nullptr;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0) {
            filein = fopen(argv[i + 1], "r");
        }
        if (strcmp(argv[i], "-o") == 0) {
            fileout = fopen(argv[i + 1], "w");
        }
    }

    // create manager
    auto *manager = createManager();

    // add Geometry Shapes From File
    ERROR_INFO errInfo = addShapesBatch(manager, filein, nullptr, nullptr);
    if (errInfo.code != ERROR_CODE::SUCCESS) {
        std::string errMsg;
        switch (errInfo.code) {
            case ERROR_CODE::INTERSECTION_EXCP:
                errMsg = "Intersection Exception";
                break;
            case ERROR_CODE::INVALID_INPUT :
                errMsg = "Invalid Input";
                break;
            case ERROR_CODE::INVALID_SHAPE :
                errMsg = "Invalid Shape";
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

    int intersectionsCount = getIntersectionsCount(manager);
    if (fileout) {
        fprintf(fileout, "%d\n", intersectionsCount);
    } else {
        printf("%d\n", intersectionsCount);
    }
    /*
    auto *xys = new gPoint[intersectionsCount * 2];
    getIntersections(manager, xys);
    for (int i = 0; i < intersectionsCount; ++i) {
        if (fileout)
            fprintf(fileout, "%.3lf,%.3lf\n", xys[i].x, xys[i].y);
        else
            printf("%.3lf,%.3lf\n", xys[i].x, xys[i].y);
    }
    */
    // close manager
//    closeManager(manager);
    return 0;
}
