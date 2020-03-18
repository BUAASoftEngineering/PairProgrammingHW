#include <iostream>
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
    ERROR_CODE err = addShapesBatch(manager, filein, nullptr, nullptr);
    if (err != ERROR_CODE::SUCCESS) {
        std::cout << "ERROR: " << err << std::endl;
    }

    int intersectionsCount = getIntersectionsCount(manager);
    if (fileout) {
        fprintf(fileout, "%d\n", intersectionsCount);
    } else {
        printf("%d\n", intersectionsCount);
    }

    // close manager
    closeManager(manager);
    return 0;
}
