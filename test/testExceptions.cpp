//
// Created by hedailan on 2020/3/20 0020.
//

#include "interface.h"
#include "gtest/gtest.h"
#include "Shapes.h"

typedef std::vector<int> case_input_t;
typedef std::vector<case_input_t> case_input_list_t;

void pushCaseByDirection(case_input_list_t &container,
                         const std::string &directionType,
                         int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    // - sub case 00: ------->    ........>  (right)
    // - sub case 01: <-------    <........  (left)
    // - sub case 02: ------->    <........  (face)
    // - sub case 03: <-------    ........>  (back)
    if (directionType == "right") {
        container.push_back(case_input_t{x1, y1, x2, y2, x3, y3, x4, y4});
    } else if (directionType == "left") {
        container.push_back(case_input_t{x2, y2, x1, y1, x4, y4, x3, y3});
    } else if (directionType == "face") {
        container.push_back(case_input_t{x1, y1, x2, y2, x4, y4, x3, y3});
    } else { // if (directionType == "back")
        container.push_back(case_input_t{x2, y2, x1, y1, x3, y3, x4, y4});
    }
}

case_input_list_t getCases(const std::string &overlapType, const std::string &directionType) {
    // case 0:   --------    .........  (divide)
    // case 1:   --------........       (cat)
    // case 2:   ------.-.-.-......     (overlap)
    case_input_list_t cases;
    if (overlapType == "divide") {
        pushCaseByDirection(cases, directionType, 1, 1, 2, 2, 3, 3, 4, 4);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, -9e4, -9e3, 9e4, 9e3, 1e5, 1e4);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, -1e5, -9e3, -1e5, 9e3, -1e5, 1e4);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, -9e4, -1e4, 9e4, -1e4, 1e5, -1e4);
    } else if (overlapType == "cat") {
        pushCaseByDirection(cases, directionType, 1, 1, 2, 2, 2, 2, 3, 3);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, -9e4, -9e3, -9e4, -9e3, 1e5, 1e4);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, -1e5, -9e3, -1e5, -9e3, -1e5, 1e4);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, -9e4, -1e4, -9e4, -1e4, 1e5, -1e4);
    } else { //if (overlapType == "overlap")
        pushCaseByDirection(cases, directionType, 1, 1, 3, 3, 2, 2, 4, 4);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, 9e4, 9e3, -9e4, -9e3, 1e5, 1e4);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, -1e5, 9e3, -1e5, -9e3, -1e5, 1e4);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, 9e4, -1e4, -9e4, -1e4, 1e5, -1e4);

        // left aligned
        pushCaseByDirection(cases, directionType, 1, 1, 3, 3, 1, 1, 4, 4);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, -9e4, -9e3, -1e5, -1e4, 1e5, 1e4);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, -1e5, -9e3, -1e5, -1e4, -1e5, 1e4);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, -9e4, -1e4, -1e5, -1e4, 1e5, -1e4);

        // right aligned
        pushCaseByDirection(cases, directionType, 1, 1, 4, 4, 2, 2, 4, 4);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, 1e5, 1e4, -9e4, -9e3, 1e5, 1e4);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, -1e5, 1e4, -1e5, -9e3, -1e5, 1e4);
        pushCaseByDirection(cases, directionType, -1e5, -1e4, 1e5, -1e4, -9e4, -1e4, 1e5, -1e4);

        // same
        pushCaseByDirection(cases, directionType, 1, 1, 4, 4, 1, 1, 4, 4);
        pushCaseByDirection(cases, directionType, -1e5, -9e4, 1e5, 9e4, -1e5, -9e4, 1e5, 9e4);
        pushCaseByDirection(cases, directionType, -1e5, -9e4, -1e5, 9e4, -1e5, -9e4, -1e5, 9e4);
        pushCaseByDirection(cases, directionType, -1e5, -9e4, 1e5, -9e4, -1e5, -9e4, 1e5, -9e4);
    }
    return cases;
}

void runCase(LineType lineTypeA, LineType lineTypeB,
             const std::string &overlapType, const std::string &directionType,
             bool expectedValid, int expectedNumIntersections, bool symmetry) {
    auto caseList = getCases(overlapType, directionType);
    for (auto &data: caseList) {
        Line a(data[0], data[1], data[2], data[3], lineTypeA);
        Line b(data[4], data[5], data[6], data[7], lineTypeB);
        auto res = intersection(a, b);
        EXPECT_EQ(std::get<1>(res), expectedNumIntersections);
        EXPECT_EQ(std::get<0>(res), expectedValid);
    }

    // test the symmetry form of given case
    if (symmetry) {
        if (directionType == "left") {
            runCase(lineTypeB, lineTypeA, overlapType, "right", expectedValid, expectedNumIntersections, false);
        } else if (directionType == "right") {
            runCase(lineTypeB, lineTypeA, overlapType, "left", expectedValid, expectedNumIntersections, false);
        } else {
            runCase(lineTypeB, lineTypeA, overlapType, directionType, expectedValid, expectedNumIntersections, false);
        }
    }
}

void runCase(LineType lineTypeA, LineType lineTypeB,
             const std::string &overlapType, const std::string &directionType,
             bool expectedValid, int expectedNumIntersections) {
    runCase(lineTypeA, lineTypeB, overlapType, directionType, expectedValid, expectedNumIntersections, true);
}


#define TEST_LINE_OVERLAP_INTERSECTED(lineTypeA, lineTypeB, overlapType, directionType, expected, numIntersections) \
TEST(ExceptionTest, lineTypeA##_##lineTypeB##_##overlapType##_##directionType) { \
    runCase((LineType::lineTypeA), (LineType::lineTypeB), #overlapType, #directionType, (expected), (numIntersections)); \
}

#define TEST_LINE_OVERLAP(lineTypeA, lineTypeB, overlapType, directionType, expected) \
TEST_LINE_OVERLAP_INTERSECTED(lineTypeA, lineTypeB, overlapType, directionType, expected, 0)


// case 0:   --------    .........  (divide)

TEST_LINE_OVERLAP(LINE, LINE, divide, left, false)

TEST_LINE_OVERLAP(LINE, LINE, divide, right, false)

TEST_LINE_OVERLAP(LINE, LINE, divide, face, false)

TEST_LINE_OVERLAP(LINE, LINE, divide, back, false)

TEST_LINE_OVERLAP(HALF_LINE, LINE, divide, left, false)

TEST_LINE_OVERLAP(HALF_LINE, LINE, divide, right, false)

TEST_LINE_OVERLAP(HALF_LINE, LINE, divide, face, false)

TEST_LINE_OVERLAP(HALF_LINE, LINE, divide, back, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, LINE, divide, left, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, LINE, divide, right, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, LINE, divide, face, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, LINE, divide, back, false)

TEST_LINE_OVERLAP(HALF_LINE, HALF_LINE, divide, left, false)

TEST_LINE_OVERLAP(HALF_LINE, HALF_LINE, divide, right, false)

TEST_LINE_OVERLAP(HALF_LINE, HALF_LINE, divide, face, false)

TEST_LINE_OVERLAP(HALF_LINE, HALF_LINE, divide, back, true)

TEST_LINE_OVERLAP(SEGMENT_LINE, HALF_LINE, divide, left, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, HALF_LINE, divide, right, true)

TEST_LINE_OVERLAP(SEGMENT_LINE, HALF_LINE, divide, face, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, HALF_LINE, divide, back, true)

TEST_LINE_OVERLAP(SEGMENT_LINE, SEGMENT_LINE, divide, left, true)

TEST_LINE_OVERLAP(SEGMENT_LINE, SEGMENT_LINE, divide, right, true)

TEST_LINE_OVERLAP(SEGMENT_LINE, SEGMENT_LINE, divide, face, true)

TEST_LINE_OVERLAP(SEGMENT_LINE, SEGMENT_LINE, divide, back, true)


// case 1:   --------........       (cat)

TEST_LINE_OVERLAP(LINE, LINE, cat, left, false)

TEST_LINE_OVERLAP(LINE, LINE, cat, right, false)

TEST_LINE_OVERLAP(LINE, LINE, cat, face, false)

TEST_LINE_OVERLAP(LINE, LINE, cat, back, false)

TEST_LINE_OVERLAP(HALF_LINE, LINE, cat, left, false)

TEST_LINE_OVERLAP(HALF_LINE, LINE, cat, right, false)

TEST_LINE_OVERLAP(HALF_LINE, LINE, cat, face, false)

TEST_LINE_OVERLAP(HALF_LINE, LINE, cat, back, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, LINE, cat, left, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, LINE, cat, right, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, LINE, cat, face, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, LINE, cat, back, false)

TEST_LINE_OVERLAP(HALF_LINE, HALF_LINE, cat, left, false)

TEST_LINE_OVERLAP(HALF_LINE, HALF_LINE, cat, right, false)

TEST_LINE_OVERLAP(HALF_LINE, HALF_LINE, cat, face, false)

TEST_LINE_OVERLAP_INTERSECTED(HALF_LINE, HALF_LINE, cat, back, true, 1);

TEST_LINE_OVERLAP(SEGMENT_LINE, HALF_LINE, cat, left, false)

TEST_LINE_OVERLAP_INTERSECTED(SEGMENT_LINE, HALF_LINE, cat, right, true, 1)

TEST_LINE_OVERLAP(SEGMENT_LINE, HALF_LINE, cat, face, false)

TEST_LINE_OVERLAP_INTERSECTED(SEGMENT_LINE, HALF_LINE, cat, back, true, 1)

TEST_LINE_OVERLAP_INTERSECTED(SEGMENT_LINE, SEGMENT_LINE, cat, left, true, 1)

TEST_LINE_OVERLAP_INTERSECTED(SEGMENT_LINE, SEGMENT_LINE, cat, right, true, 1)

TEST_LINE_OVERLAP_INTERSECTED(SEGMENT_LINE, SEGMENT_LINE, cat, face, true, 1)

TEST_LINE_OVERLAP_INTERSECTED(SEGMENT_LINE, SEGMENT_LINE, cat, back, true, 1)

// case 2:   ------.-.-.-......     (overlap)

TEST_LINE_OVERLAP(LINE, LINE, overlap, left, false)

TEST_LINE_OVERLAP(LINE, LINE, overlap, right, false)

TEST_LINE_OVERLAP(LINE, LINE, overlap, face, false)

TEST_LINE_OVERLAP(LINE, LINE, overlap, back, false)

TEST_LINE_OVERLAP(HALF_LINE, LINE, overlap, left, false)

TEST_LINE_OVERLAP(HALF_LINE, LINE, overlap, right, false)

TEST_LINE_OVERLAP(HALF_LINE, LINE, overlap, face, false)

TEST_LINE_OVERLAP(HALF_LINE, LINE, overlap, back, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, LINE, overlap, left, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, LINE, overlap, right, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, LINE, overlap, face, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, LINE, overlap, back, false)

TEST_LINE_OVERLAP(HALF_LINE, HALF_LINE, overlap, left, false)

TEST_LINE_OVERLAP(HALF_LINE, HALF_LINE, overlap, right, false)

TEST_LINE_OVERLAP(HALF_LINE, HALF_LINE, overlap, face, false)

TEST_LINE_OVERLAP(HALF_LINE, HALF_LINE, overlap, back, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, HALF_LINE, overlap, left, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, HALF_LINE, overlap, right, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, HALF_LINE, overlap, face, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, HALF_LINE, overlap, back, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, SEGMENT_LINE, overlap, left, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, SEGMENT_LINE, overlap, right, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, SEGMENT_LINE, overlap, face, false)

TEST_LINE_OVERLAP(SEGMENT_LINE, SEGMENT_LINE, overlap, back, false)


// circle circle overlap
TEST(ExceptionTest, CIRCLE_CIRCLE) {
    auto res = intersection(Circle(10, 35, 13), Circle(10, 35, 13));
    EXPECT_EQ(std::get<1>(res), 0);
    EXPECT_FALSE(std::get<0>(res));
}

// interface
TEST(ExceptionTest, Interface) {
    auto mgr = createManager();
    auto res1 = addShape(mgr, 'L', 0, 0, 1, 1, nullptr, nullptr);
    auto res2 = addShape(mgr, 'L', 0, 0, 1, 1, nullptr, nullptr);
    EXPECT_EQ(res1, ERROR_CODE::SUCCESS);
    EXPECT_EQ(res2, ERROR_CODE::INTERSECTION_EXCP);
}

TEST(ExceptionTest, InvalidInput1) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/invalid_input1.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr), ERROR_CODE::INVALID_INPUT);
    EXPECT_EQ(getIntersectionsCount(mng), 3);
    closeManager(mng);
}

TEST(ExceptionTest, InvalidInput2) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/invalid_input2.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr), ERROR_CODE::INVALID_INPUT);
    EXPECT_EQ(getIntersectionsCount(mng), 0);
    closeManager(mng);
}

TEST(ExceptionTest, InvalidInput3) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/invalid_input3.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr), ERROR_CODE::INVALID_INPUT);
    EXPECT_EQ(getIntersectionsCount(mng), 1);
    closeManager(mng);
}

TEST(ExceptionTest, InvalidInput4) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/invalid_input4.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr), ERROR_CODE::INVALID_INPUT);
    EXPECT_EQ(getIntersectionsCount(mng), 0);
    closeManager(mng);
}

TEST(ExceptionTest, InvalidInput5) {
    gManager *mng = createManager();
    FILE *filein = fopen("../data/invalid_input5.txt", "r");
    EXPECT_EQ(addShapesBatch(mng, filein, nullptr, nullptr), ERROR_CODE::INVALID_INPUT);
    EXPECT_EQ(getIntersectionsCount(mng), 0);
    closeManager(mng);
}