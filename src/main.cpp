#include <iostream>
#include <cstring>
#include <unordered_set>
#include "Shapes.h"
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

    // init instance
    auto *inst = init_geo_instance();
    int err = addBatchFromFile(inst, filein, nullptr, nullptr);
    if(err != ERROR_CODE::SUCCESS){
        std::cout << "ERROR: " << err << std::endl;
    }

    int objCount = getNumIntersection(inst);
    std::cout << objCount << std::endl;
    if (fileout) {
        fprintf(fileout, "%d", objCount);
    }

    close_geo_instance(inst);
    return 0;
}
