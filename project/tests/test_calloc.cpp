// Copyright 2021 nat-s.skv@mail.ru
extern "C" {
#include <matrix.h>
}

#include <gtest/gtest.h>
#include <cstring>
#include <fstream>

class FixtureCalloc : public ::testing::Test {
protected:
    Matrix_t mtrx = {0u, 0u, nullptr};

    void SetUp(size_t rows, size_t cols) {
        mtrx.rows = rows;
        mtrx.cols = cols;
        mtrx.elems = calloc_mtrx(mtrx.rows, mtrx.cols);
        ASSERT_TRUE(mtrx.elems || !mtrx.rows || !mtrx.cols);
        for (size_t row = 0u; row < mtrx.rows; ++row) {
            EXPECT_NE(mtrx.elems[row], nullptr);
            for (size_t col = 0u; col < mtrx.cols; ++col) {
                EXPECT_EQ(mtrx.elems[row][col], 0);
            }
        }
    }

    void TearDown() {
        free_mtrx(mtrx.elems, mtrx.rows);
    }
};

TEST(CALLOC, CALLOC_0_BYTES) {
    EXPECT_EQ(calloc_mtrx(0u, 4u), nullptr);
    EXPECT_EQ(calloc_mtrx(4u, 0u), nullptr);
    EXPECT_EQ(calloc_mtrx(0u, 0u), nullptr);
}

TEST(CALLOC, CALLOC_SIZE_MAX_INTS) {
    const size_t size_max = SIZE_MAX;
    EXPECT_EQ(calloc_mtrx(size_max, 1u), nullptr);
    EXPECT_EQ(calloc_mtrx(1u, size_max), nullptr);
}

TEST_F(FixtureCalloc, CALLOC_4X4) {
    FixtureCalloc::SetUp(4u, 4u);
}

TEST_F(FixtureCalloc, CALLOC_1X5) {
    FixtureCalloc::SetUp(1u, 5u);
}

TEST_F(FixtureCalloc, CALLOC_100X100) {
    FixtureCalloc::SetUp(100u, 100u);
}
