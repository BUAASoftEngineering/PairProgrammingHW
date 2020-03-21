//
// Created by xiangweilai on 2020/3/19.
//

#include "interface.h"
#include "gtest/gtest.h"

TEST(CoreTest, create_clean_close_gets) {
    gManager *mng = createManager();
    gPoint xys[10];

    EXPECT_EQ(addShape(mng, 'L', 0, 0, 1, 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 0);
    EXPECT_EQ(addShape(mng, 'L', 0, 0, -1, 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 1);
    for (auto &xy : xys)
        xy = gPoint{1234.56, 1234.56};
    getIntersections(mng, xys);
    EXPECT_DOUBLE_EQ(xys[0].x, 0);
    EXPECT_DOUBLE_EQ(xys[0].y, 0);
    EXPECT_DOUBLE_EQ(xys[1].x, 1234.56);
    EXPECT_EQ(getIntersectionsCount(mng), 1);

    EXPECT_EQ(addShape(mng, 'C', 3, 4, 5, 1e5 - 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(addShape(mng, 'R', -3, -4, -5, -6, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(addShape(mng, 'S', 3, 4, 5, 6, nullptr, nullptr), ERROR_CODE::SUCCESS);
    gShape lcs[10];
    for (auto &lc : lcs)
        lc = gShape{'X', -1, -1, -1, -1};
    getGeometricShapes(mng, lcs);
    EXPECT_EQ(lcs[0].type, 'L');
    EXPECT_EQ(lcs[0].x1 + lcs[0].y1 + lcs[0].x2 + lcs[0].y2, 0 + 0 + 1 + 1);
    EXPECT_EQ(lcs[1].type, 'L');
    EXPECT_EQ(lcs[1].x1 + lcs[1].y1 + lcs[1].x2 + lcs[1].y2, 0 + 0 + -1 + 1);
    EXPECT_EQ(lcs[2].type, 'C');
    EXPECT_EQ(lcs[2].x1 + lcs[2].y1 + lcs[2].x2 + lcs[2].y2, 3 + 4 + 5 + -1);
    EXPECT_EQ(lcs[3].type, 'R');
    EXPECT_EQ(lcs[3].x1 + lcs[3].y1 + lcs[3].x2 + lcs[3].y2, -3 + -4 + -5 + -6);
    EXPECT_EQ(lcs[4].type, 'S');
    EXPECT_EQ(lcs[4].x1 + lcs[4].y1 + lcs[4].x2 + lcs[4].y2, 3 + 4 + 5 + 6);
    EXPECT_EQ(lcs[5].type, 'X');

    cleanManager(mng);

    EXPECT_EQ(addShape(mng, 'L', 0, 0, 1, 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 0);
    EXPECT_EQ(addShape(mng, 'L', 0, 0, -1, 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 1);
    for (auto &xy : xys)
        xy = gPoint{6543.21, 6543.21};
    getIntersections(mng, xys);
    EXPECT_DOUBLE_EQ(xys[0].x, 0);
    EXPECT_DOUBLE_EQ(xys[0].x, 0);
    EXPECT_DOUBLE_EQ(xys[1].x, 6543.21);
    EXPECT_EQ(getIntersectionsCount(mng), 1);

    closeManager(mng);
}

TEST(CoreTest, add_getCount) {
    gManager *mng = createManager();
    // y = x
    EXPECT_EQ(addShape(mng, 'L', 0, 0, 1, 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 0);
    // y = -x
    EXPECT_EQ(addShape(mng, 'L', 0, 0, -1, 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 1);
    // y = -1
    EXPECT_EQ(addShape(mng, 'L', 0, -1, 1, -1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 3);
    // y = 1
    EXPECT_EQ(addShape(mng, 'L', 0, 1, 1, 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 5);
    // x = -1
    EXPECT_EQ(addShape(mng, 'L', -1, 0, -1, 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 5);
    // x = 1
    EXPECT_EQ(addShape(mng, 'L', 1, 0, 1, 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 5);
    // x = 0, y >= 1
    EXPECT_EQ(addShape(mng, 'R', 0, 1, 0, 10, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 6);
    // y = 0, x >= 1
    EXPECT_EQ(addShape(mng, 'R', 1, 0, 10, 0, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 7);
    // (-1, 0) & (0, -1)
    EXPECT_EQ(addShape(mng, 'S', -1, 0, 0, -1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 10);
    // circle(1, 1, 1)
    EXPECT_EQ(addShape(mng, 'C', 1, 1, 1, 1e5 - 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 14);
    // circle(-1, -1, 1)
    EXPECT_EQ(addShape(mng, 'C', -1, -1, 1, 1e5 - 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 18);
    // circle(1, -1, 1)
    EXPECT_EQ(addShape(mng, 'C', 1, -1, 1, 1e5 - 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 22);
    // circle(-1, 1, 1)
    EXPECT_EQ(addShape(mng, 'C', -1, 1, 1, 1e5 - 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 26);
    closeManager(mng);
}

TEST(CoreTest, add_return) {
    gManager *mng = createManager();
    gPoint xys[100];
    for (auto &xy : xys)
        xy = gPoint{1234.56, 1234.56};
    int length = 0;
    // y = x
    EXPECT_EQ(addShape(mng, 'L', 0, 0, 1, 1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 0);
    EXPECT_DOUBLE_EQ(xys[length + 1].x, 1234.56);
    // y = -x
    EXPECT_EQ(addShape(mng, 'L', 0, 0, -1, 1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 1);
    EXPECT_DOUBLE_EQ(xys[0].x, 0);
    EXPECT_DOUBLE_EQ(xys[0].y, 0);
    EXPECT_DOUBLE_EQ(xys[length + 1].x, 1234.56);
    // y = -1
    EXPECT_EQ(addShape(mng, 'L', 0, -1, 1, -1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 3);
    EXPECT_DOUBLE_EQ(xys[1].x, -1);
    EXPECT_DOUBLE_EQ(xys[1].y, -1);
    EXPECT_DOUBLE_EQ(xys[2].x, 1);
    EXPECT_DOUBLE_EQ(xys[2].y, -1);
    EXPECT_DOUBLE_EQ(xys[length + 1].x, 1234.56);
    // y = 1
    EXPECT_EQ(addShape(mng, 'L', 0, 1, 1, 1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 5);
    EXPECT_DOUBLE_EQ(xys[3].x, 1);
    EXPECT_DOUBLE_EQ(xys[3].y, 1);
    EXPECT_DOUBLE_EQ(xys[4].x, -1);
    EXPECT_DOUBLE_EQ(xys[4].y, 1);
    EXPECT_DOUBLE_EQ(xys[length + 1].x, 1234.56);
    // x = -1
    EXPECT_EQ(addShape(mng, 'L', -1, 0, -1, 1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 5);
    EXPECT_DOUBLE_EQ(xys[length + 1].x, 1234.56);
    // x = 1
    EXPECT_EQ(addShape(mng, 'L', 1, 0, 1, 1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 5);
    EXPECT_DOUBLE_EQ(xys[length + 1].x, 1234.56);
    // x = 0, y >= 1
    EXPECT_EQ(addShape(mng, 'R', 0, 1, 0, 10, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 6);
    EXPECT_DOUBLE_EQ(xys[5].x, 0);
    EXPECT_DOUBLE_EQ(xys[5].y, 1);
    EXPECT_DOUBLE_EQ(xys[length + 1].x, 1234.56);
    // y = 0, x >= 1
    EXPECT_EQ(addShape(mng, 'R', 1, 0, 10, 0, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 7);
    EXPECT_DOUBLE_EQ(xys[6].x, 1);
    EXPECT_DOUBLE_EQ(xys[6].y, 0);
    EXPECT_DOUBLE_EQ(xys[length + 1].x, 1234.56);
    // (-1, 0) & (0, -1)
    EXPECT_EQ(addShape(mng, 'S', -1, 0, 0, -1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 10);
    EXPECT_DOUBLE_EQ(xys[7].x, -0.5);
    EXPECT_DOUBLE_EQ(xys[7].y, -0.5);
    EXPECT_DOUBLE_EQ(xys[8].x, 0);
    EXPECT_DOUBLE_EQ(xys[8].y, -1);
    EXPECT_DOUBLE_EQ(xys[9].x, -1);
    EXPECT_DOUBLE_EQ(xys[9].y, 0);
    EXPECT_DOUBLE_EQ(xys[length + 1].x, 1234.56);
    // circle(1, 1, 1)
    EXPECT_EQ(addShape(mng, 'C', 1, 1, 1, 1e5 - 1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 14);
    EXPECT_DOUBLE_EQ(xys[length + 1].x, 1234.56);
    // circle(-1, -1, 1)
    EXPECT_EQ(addShape(mng, 'C', -1, -1, 1, 1e5 - 1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 18);
    EXPECT_DOUBLE_EQ(xys[length + 1].x, 1234.56);
    // circle(1, -1, 1)
    EXPECT_EQ(addShape(mng, 'C', 1, -1, 1, 1e5 - 1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 22);
    EXPECT_DOUBLE_EQ(xys[length + 1].x, 1234.56);
    // circle(-1, 1, 1)
    EXPECT_EQ(addShape(mng, 'C', -1, 1, 1, 1e5 - 1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 26);
    EXPECT_DOUBLE_EQ(xys[length + 1].x, 1234.56);

    // refresh storage position from start: xys[0...1]
    length = 0;
    EXPECT_EQ(addShape(mng, 'S', 6, 10, 10, 6, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_DOUBLE_EQ(xys[0].x, 8);
    EXPECT_DOUBLE_EQ(xys[0].y, 8);
    EXPECT_DOUBLE_EQ(xys[1].x, -1);
    closeManager(mng);
}

TEST(CoreTest, addFromStd_or_File_getCount) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/499361.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr).code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 499361);

    cleanManager(mng);
    printf("plz input:\n4\nC 3 3 3\nS 2 4 3 2\nL -1 4 5 2\nR 2 5 -1 2\n");
    EXPECT_EQ(addShapesBatch(mng, nullptr, nullptr, nullptr).code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 5);
    closeManager(mng);
}

TEST(CoreTest, precision1) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/9423886.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr).code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 9423886);
    closeManager(mng);
}

TEST(CoreTest, precision2) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/4.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr).code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 4);
    closeManager(mng);
}

TEST(CoreTest, precision3) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/26.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr).code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 26);
    closeManager(mng);
}

TEST(CoreTest, precision4) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/6327238.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr).code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 6327238);
    closeManager(mng);
}

TEST(CoreTest, precision5) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/9202253.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr).code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 9202253);
    closeManager(mng);
}