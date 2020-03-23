//
// Created by xiangweilai on 2020/3/23.
//

#include "StdInterface.h"
#include "gtest/gtest.h"

TEST(gCoreTest, add_clean_deleteFigure__addShapes__updates) {
    auto *fig = addFigure();

    EXPECT_EQ(addShapeToFigure(fig, gShape{'L', 0, 0, 1, 1}).code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 0);
    EXPECT_EQ(addShapeToFigure(fig, {'L', 0, 0, -1, 1}).code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 1);
    updatePoints(fig);
    EXPECT_DOUBLE_EQ(fig->points[0].x, 0);
    EXPECT_DOUBLE_EQ(fig->points[0].y, 0);
    EXPECT_EQ(getPointsCount(fig), 1);

    EXPECT_EQ(addShapeToFigure(fig, gShape{'C', 3, 4, 5, int(1e5 - 1)}).code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(addShapeToFigure(fig, gShape{'R', -3, -4, -5, -6}).code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(addShapeToFigure(fig, gShape{'S', 3, 4, 5, 6}).code, ERROR_CODE::SUCCESS);
    updateShapes(fig);
    EXPECT_EQ(fig->shapes[0].type, 'L');
    EXPECT_EQ(fig->shapes[0].x1 + fig->shapes[0].y1 + fig->shapes[0].x2 + fig->shapes[0].y2, 0 + 0 + 1 + 1);
    EXPECT_EQ(fig->shapes[1].type, 'L');
    EXPECT_EQ(fig->shapes[1].x1 + fig->shapes[1].y1 + fig->shapes[1].x2 + fig->shapes[1].y2, 0 + 0 + -1 + 1);
    EXPECT_EQ(fig->shapes[2].type, 'C');
    EXPECT_EQ(fig->shapes[2].x1 + fig->shapes[2].y1 + fig->shapes[2].x2 + fig->shapes[2].y2, 3 + 4 + 5 + -1);
    EXPECT_EQ(fig->shapes[3].type, 'R');
    EXPECT_EQ(fig->shapes[3].x1 + fig->shapes[3].y1 + fig->shapes[3].x2 + fig->shapes[3].y2, -3 + -4 + -5 + -6);
    EXPECT_EQ(fig->shapes[4].type, 'S');
    EXPECT_EQ(fig->shapes[4].x1 + fig->shapes[4].y1 + fig->shapes[4].x2 + fig->shapes[4].y2, 3 + 4 + 5 + 6);

    cleanFigure(fig);

    EXPECT_EQ(addShapeToFigure(fig, gShape{'L', 0, 0, 1, 1}).code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 0);
    EXPECT_EQ(addShapeToFigure(fig, gShape{'L', 0, 0, -1, 1}).code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 1);
    updatePoints(fig);
    EXPECT_DOUBLE_EQ(fig->points[0].x, 0);
    EXPECT_DOUBLE_EQ(fig->points[0].x, 0);
    EXPECT_EQ(getPointsCount(fig), 1);

    deleteFigure(fig);
}

TEST(gCoreTest, addShapesString__gets) {
    auto *fig = addFigure();

    // y = x
    EXPECT_EQ(addShapeToFigureString(fig, "L 0 0 1 1").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 0);
    // y = -x
    EXPECT_EQ(addShapeToFigureString(fig, "  L 0 0 -1 1").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 1);
    // y = -1
    EXPECT_EQ(addShapeToFigureString(fig, "L  0 -1 1 -1").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 3);
    // y = 1
    EXPECT_EQ(addShapeToFigureString(fig, "L 0  1 1 1").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 5);
    // x = -1
    EXPECT_EQ(addShapeToFigureString(fig, "L -1 0  -1 1").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 5);
    // x = 1
    EXPECT_EQ(addShapeToFigureString(fig, "L 1 0 1  1").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 5);
    // x = 0, y >= 1
    EXPECT_EQ(addShapeToFigureString(fig, "R 0 1 0 10 ").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 6);
    // y = 0, x >= 1
    EXPECT_EQ(addShapeToFigureString(fig, " R  1  0  10  0 ").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 7);
    // (-1, 0) & (0, -1)
    EXPECT_EQ(addShapeToFigureString(fig, "  \n  S    -1  \t 0  0  \n -1").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 10);
    // circle(1, 1, 1)
    EXPECT_EQ(addShapeToFigureString(fig, "C 1 1 1 99999").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 14);
    // circle(-1, -1, 1)
    EXPECT_EQ(addShapeToFigureString(fig, "C -1 -1 1 99999").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 18);
    // circle(1, -1, 1)
    EXPECT_EQ(addShapeToFigureString(fig, "C 1 -1 1 99999").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 22);
    // circle(-1, 1, 1)
    EXPECT_EQ(addShapeToFigureString(fig, "C -1 1 1 99999").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 26);

    EXPECT_EQ(getShapesCount(fig), 13);
    cleanFigure(fig);

    EXPECT_EQ(addShapeToFigureString(fig, "L 0 0 1 1").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(addShapeToFigureString(fig, " ").code, ERROR_CODE::WRONG_FORMAT);
    EXPECT_EQ(addShapeToFigureString(fig, " 0 0 1 1").code, ERROR_CODE::WRONG_FORMAT);
    EXPECT_EQ(addShapeToFigureString(fig, "L").code, ERROR_CODE::WRONG_FORMAT);
    EXPECT_EQ(addShapeToFigureString(fig, "L 0 ").code, ERROR_CODE::WRONG_FORMAT);
    EXPECT_EQ(addShapeToFigureString(fig, "L 0 0").code, ERROR_CODE::WRONG_FORMAT);
    EXPECT_EQ(addShapeToFigureString(fig, "L 0 0 1 ").code, ERROR_CODE::WRONG_FORMAT);
    EXPECT_EQ(getShapesCount(fig), 1);
    deleteFigure(fig);
}

TEST(gCoreTest, addShapesStdin_File__gets) {
    auto *fig = addFigure();
    FILE *filein = fopen("../data/499361.txt", "r");
    EXPECT_EQ(addShapesToFigureFile(fig, "../data/499361.txt").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 499361);

    cleanFigure(fig);
    printf("plz input:\n4\nC 3 3 3\nS 2 4 3 2\nL -1 4 5 2\nR 2 5 -1 2\n");
    EXPECT_EQ(addShapesToFigureStdin(fig).code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 5);
    deleteFigure(fig);
}

TEST(gCoreTest, remove__gets) {
    auto *fig = addFigure();
    FILE *filein = fopen("../data/499361.txt", "r");
    EXPECT_EQ(addShapesToFigureFile(fig, "../data/499361.txt").code, ERROR_CODE::SUCCESS);
    EXPECT_EQ(getPointsCount(fig), 499361);
    EXPECT_EQ(getShapesCount(fig), 1000);

    for (int i = 0; i < 100; ++i)
        removeShapeByIndex(fig, 0);
    EXPECT_EQ(getPointsCount(fig), 404423);
    EXPECT_EQ(getShapesCount(fig), 900);
    deleteFigure(fig);
}