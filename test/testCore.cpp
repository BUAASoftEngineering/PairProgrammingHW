//
// Created by xiangweilai on 2020/3/19.
//

#include "interface.h"
#include "gtest/gtest.h"

TEST(CoreTest, create_clean_close_gets) {
    gManager *mng = createManager();
    double xys[10];

    EXPECT_EQ(addShape(mng, 'L', 0, 0, 1, 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 0);
    EXPECT_EQ(addShape(mng, 'L', 0, 0, -1, 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 1);
    for (double &xy : xys)
        xy = 1234.56;
    getIntersections(mng, xys);
    EXPECT_DOUBLE_EQ(xys[0], 0);
    EXPECT_DOUBLE_EQ(xys[1], 0);
    EXPECT_DOUBLE_EQ(xys[2], 1234.56);
    EXPECT_EQ(getIntersectionsCount(mng), 1);

    cleanManager(mng);

    EXPECT_EQ(addShape(mng, 'L', 0, 0, 1, 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 0);
    EXPECT_EQ(addShape(mng, 'L', 0, 0, -1, 1, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 1);
    for (double &xy : xys)
        xy = 6543.21;
    getIntersections(mng, xys);
    EXPECT_DOUBLE_EQ(xys[0], 0);
    EXPECT_DOUBLE_EQ(xys[1], 0);
    EXPECT_DOUBLE_EQ(xys[2], 6543.21);
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
    EXPECT_EQ(addShape(mng, 'C', 1, 1, 1, INT_MAX, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 14);
    // circle(-1, -1, 1)
    EXPECT_EQ(addShape(mng, 'C', -1, -1, 1, INT_MAX, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 18);
    // circle(1, -1, 1)
    EXPECT_EQ(addShape(mng, 'C', 1, -1, 1, INT_MAX, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 22);
    // circle(-1, 1, 1)
    EXPECT_EQ(addShape(mng, 'C', -1, 1, 1, INT_MAX, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 26);
    closeManager(mng);
}

TEST(CoreTest, add_return) {
    gManager *mng = createManager();
    double xys[100];
    for (double &xy : xys)
        xy = 1234.56;
    int length = 0;
    // y = x
    EXPECT_EQ(addShape(mng, 'L', 0, 0, 1, 1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 0 * 2);
    EXPECT_DOUBLE_EQ(xys[length + 1], 1234.56);
    // y = -x
    EXPECT_EQ(addShape(mng, 'L', 0, 0, -1, 1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 1 * 2);
    EXPECT_DOUBLE_EQ(xys[0], 0);
    EXPECT_DOUBLE_EQ(xys[1], 0);
    EXPECT_DOUBLE_EQ(xys[length + 1], 1234.56);
    // y = -1
    EXPECT_EQ(addShape(mng, 'L', 0, -1, 1, -1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 3 * 2);
    EXPECT_DOUBLE_EQ(xys[2], -1);
    EXPECT_DOUBLE_EQ(xys[3], -1);
    EXPECT_DOUBLE_EQ(xys[4], 1);
    EXPECT_DOUBLE_EQ(xys[5], -1);
    EXPECT_DOUBLE_EQ(xys[length + 1], 1234.56);
    // y = 1
    EXPECT_EQ(addShape(mng, 'L', 0, 1, 1, 1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 5 * 2);
    EXPECT_DOUBLE_EQ(xys[6], 1);
    EXPECT_DOUBLE_EQ(xys[7], 1);
    EXPECT_DOUBLE_EQ(xys[8], -1);
    EXPECT_DOUBLE_EQ(xys[9], 1);
    EXPECT_DOUBLE_EQ(xys[length + 1], 1234.56);
    // x = -1
    EXPECT_EQ(addShape(mng, 'L', -1, 0, -1, 1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 5 * 2);
    EXPECT_DOUBLE_EQ(xys[length + 1], 1234.56);
    // x = 1
    EXPECT_EQ(addShape(mng, 'L', 1, 0, 1, 1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 5 * 2);
    EXPECT_DOUBLE_EQ(xys[length + 1], 1234.56);
    // x = 0, y >= 1
    EXPECT_EQ(addShape(mng, 'R', 0, 1, 0, 10, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 6 * 2);
    EXPECT_DOUBLE_EQ(xys[10], 0);
    EXPECT_DOUBLE_EQ(xys[11], 1);
    EXPECT_DOUBLE_EQ(xys[length + 1], 1234.56);
    // y = 0, x >= 1
    EXPECT_EQ(addShape(mng, 'R', 1, 0, 10, 0, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 7 * 2);
    EXPECT_DOUBLE_EQ(xys[12], 1);
    EXPECT_DOUBLE_EQ(xys[13], 0);
    EXPECT_DOUBLE_EQ(xys[length + 1], 1234.56);
    // (-1, 0) & (0, -1)
    EXPECT_EQ(addShape(mng, 'S', -1, 0, 0, -1, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 10 * 2);
    EXPECT_DOUBLE_EQ(xys[14], -0.5);
    EXPECT_DOUBLE_EQ(xys[15], -0.5);
    EXPECT_DOUBLE_EQ(xys[16], 0);
    EXPECT_DOUBLE_EQ(xys[17], -1);
    EXPECT_DOUBLE_EQ(xys[18], -1);
    EXPECT_DOUBLE_EQ(xys[19], 0);
    EXPECT_DOUBLE_EQ(xys[length + 1], 1234.56);
    // circle(1, 1, 1)
    EXPECT_EQ(addShape(mng, 'C', 1, 1, 1, INT_MAX, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 14 * 2);
    EXPECT_DOUBLE_EQ(xys[length + 1], 1234.56);
    // circle(-1, -1, 1)
    EXPECT_EQ(addShape(mng, 'C', -1, -1, 1, INT_MAX, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 18 * 2);
    EXPECT_DOUBLE_EQ(xys[length + 1], 1234.56);
    // circle(1, -1, 1)
    EXPECT_EQ(addShape(mng, 'C', 1, -1, 1, INT_MAX, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 22 * 2);
    EXPECT_DOUBLE_EQ(xys[length + 1], 1234.56);
    // circle(-1, 1, 1)
    EXPECT_EQ(addShape(mng, 'C', -1, 1, 1, INT_MAX, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_EQ(length, 26 * 2);
    EXPECT_DOUBLE_EQ(xys[length + 1], 1234.56);

    // refresh storage position from start: xys[0...1]
    length = 0;
    EXPECT_EQ(addShape(mng, 'S', 6, 10, 10, 6, xys, &length), ERROR_CODE::SUCCESS);
    EXPECT_DOUBLE_EQ(xys[0], 8);
    EXPECT_DOUBLE_EQ(xys[1], 8);
    EXPECT_DOUBLE_EQ(xys[2], -1);
    closeManager(mng);
}

TEST(CoreTest, addFromStd_or_File_getCount) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/499361.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 499361);

    cleanManager(mng);
    printf("plz input:\n4\nC 3 3 3\nS 2 4 3 2\nL -1 4 5 2\nR 2 5 -1 2\n");
    EXPECT_EQ(addShapesBatch(mng, nullptr, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 5);
    closeManager(mng);
}

TEST(CoreTest, precision1) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/9423886.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 9423886);
    closeManager(mng);
}

TEST(CoreTest, precision2) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/4.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 4);
    closeManager(mng);
}

TEST(CoreTest, precision3) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/26.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 26);
    closeManager(mng);
}

TEST(CoreTest, precision4) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/6327238.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 6327238);
    closeManager(mng);
}

TEST(CoreTest, precision5) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/9202253.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr), ERROR_CODE::SUCCESS);
    EXPECT_EQ(getIntersectionsCount(mng), 9202253);
    closeManager(mng);
}

TEST(CoreTest, invalid_shape) {
    gManager *mng = createManager();
    EXPECT_EQ(addShape(mng, '?', 6, 10, 10, 6, nullptr, nullptr), ERROR_CODE::INVALID_SHAPE);
    EXPECT_EQ(getIntersectionsCount(mng), 0);

    FILE *filein = fopen("../data/invalid_shape.txt", "r");
    double xys[10];
    int length = 0;
    EXPECT_EQ(addShapesBatch(mng, filein, xys, &length), ERROR_CODE::INVALID_SHAPE);
    EXPECT_EQ(getIntersectionsCount(mng), 3);
    EXPECT_EQ(length, 3 * 2);
    EXPECT_DOUBLE_EQ(xys[0], 1);
    EXPECT_DOUBLE_EQ(xys[1], 1);
    EXPECT_DOUBLE_EQ(xys[2], 0);
    EXPECT_DOUBLE_EQ(xys[3], 1);
    EXPECT_DOUBLE_EQ(xys[4], 1);
    EXPECT_DOUBLE_EQ(xys[5], 0);
    closeManager(mng);
}