// Copyright 2021 nat-s.skv@mail.ru
extern "C" {
#include <matrix.h>
}

#include <gtest/gtest.h>
#include <cstring>
#include <fstream>

class FixtureMultiply : public ::testing::Test {
protected:
    static const int bufSize = 100;

    void SetUp(const char *finPath, const char *foutPath, const char *foutExpPath) {
        FILE *fin = fopen(finPath, "r");
        FILE *fout = fopen(foutPath, "w+");
        FILE *foutExp = fopen(foutExpPath, "r");
        ASSERT_TRUE(fin && fout && foutExp);
        Matrix_t mtrx1 = {0u, 0u, NULL};
        EXPECT_EQ(fscanf_mtrx_sizes(fin, &mtrx1), 0);
        mtrx1.elems = calloc_mtrx(mtrx1.rows, mtrx1.cols);
        ASSERT_TRUE(mtrx1.elems || !mtrx1.rows || !mtrx1.cols);
        EXPECT_EQ(fscanf_mtrx(fin, &mtrx1), 0);

        Matrix_t mtrx2 = {0u, 0u, NULL};
        EXPECT_EQ(fscanf_mtrx_sizes(fin, &mtrx2), 0);
        mtrx2.elems = calloc_mtrx(mtrx2.rows, mtrx2.cols);
        ASSERT_TRUE(mtrx2.elems || !mtrx2.rows || !mtrx2.cols);
        EXPECT_EQ(fscanf_mtrx(fin, &mtrx2), 0);

        Matrix_t mtrx_product = {0u, 0u, NULL};
        multiply_matrices(&mtrx1, &mtrx2, &mtrx_product);
        EXPECT_EQ(fprintf_mtrx(fout, &mtrx_product), 0);

        fseek(fout, 0, SEEK_SET);
        CompareFiles(fout, foutExp);
        free_mtrx(mtrx1.elems, mtrx1.rows);
        free_mtrx(mtrx2.elems, mtrx2.rows);
        free_mtrx(mtrx_product.elems, mtrx_product.rows);

        EXPECT_EQ(fclose(fin), 0);
        EXPECT_EQ(fclose(fout), 0);
        EXPECT_EQ(fclose(foutExp), 0);
    }

    void CompareFiles(FILE *f1, FILE *f2) {
        char bufFin[bufSize];
        char bufFout[bufSize];
        while (!feof(f1) && !feof(f2)) {
            fgets(bufFin, bufSize, f1);
            fgets(bufFout, bufSize, f2);
            EXPECT_EQ(std::memcmp(bufFin, bufFout, strlen(bufFin)), 0);
        }
        EXPECT_TRUE(feof(f1) && feof(f2));
    }

    void TearDown() {
    }
};

TEST_F(FixtureMultiply, MULTIPLY_2X2_X_2X4) {
    FixtureMultiply::SetUp(PROJECT_PATH "tests/input_emulator/multiply_matrices/2x2_x_2x4/fin",
                           PROJECT_PATH "tests/input_emulator/multiply_matrices/2x2_x_2x4/fout",
                           PROJECT_PATH "tests/input_emulator/multiply_matrices/2x2_x_2x4/fexp");
}

TEST_F(FixtureMultiply, MULTIPLY_2X2_X_2X7) {
    FixtureMultiply::SetUp(PROJECT_PATH "tests/input_emulator/multiply_matrices/2x2_x_2x7/fin",
                           PROJECT_PATH "tests/input_emulator/multiply_matrices/2x2_x_2x7/fout",
                           PROJECT_PATH "tests/input_emulator/multiply_matrices/2x2_x_2x7/fexp");
}

TEST_F(FixtureMultiply, MULTIPLY_5X6_X_6X7) {
    FixtureMultiply::SetUp(PROJECT_PATH "tests/input_emulator/multiply_matrices/5x6_x_6x7/fin",
                           PROJECT_PATH "tests/input_emulator/multiply_matrices/5x6_x_6x7/fout",
                           PROJECT_PATH "tests/input_emulator/multiply_matrices/5x6_x_6x7/fexp");
}


TEST(MULTIPLY_MATRICES, MULTIPLY_INVALID_ARGS) {
    Matrix_t mtrx_2x2 = {2u, 2u, NULL};
    Matrix_t mtrx_2x1 = {2u, 1u, NULL};
    Matrix_t mtrx_1x2 = {1u, 2u, NULL};
    ASSERT_TRUE(mtrx_2x2.elems = calloc_mtrx(mtrx_2x2.rows, mtrx_2x2.cols));
    ASSERT_TRUE(mtrx_2x1.elems = calloc_mtrx(mtrx_2x1.rows, mtrx_2x1.cols));
    ASSERT_TRUE(mtrx_1x2.elems = calloc_mtrx(mtrx_1x2.rows, mtrx_1x2.cols));
    Matrix_t mtrx_2x2_empty = {2u, 2u, NULL};
    Matrix_t mtrx_2x1_empty = {2u, 1u, NULL};
    Matrix_t mtrx_1x2_empty = {1u, 2u, NULL};

    EXPECT_NE(multiply_matrices(NULL, &mtrx_2x2, &mtrx_1x2_empty), 0);
    EXPECT_NE(multiply_matrices(&mtrx_2x2, NULL, &mtrx_2x1_empty), 0);
    EXPECT_NE(multiply_matrices(&mtrx_2x2, &mtrx_2x1, NULL), 0);

    EXPECT_NE(multiply_matrices(&mtrx_2x1_empty, &mtrx_1x2, &mtrx_2x2_empty), 0);
    EXPECT_NE(multiply_matrices(&mtrx_2x2, &mtrx_2x1_empty, &mtrx_2x1_empty), 0);

    // multiplication is not defined for matrices lhs_mtrx->cols != rhs_mtrx->rows
    EXPECT_NE(multiply_matrices(&mtrx_2x2, &mtrx_1x2, &mtrx_2x2_empty), 0);
    EXPECT_NE(multiply_matrices(&mtrx_2x1, &mtrx_2x2, &mtrx_2x2_empty), 0);

    free_mtrx(mtrx_2x2.elems, mtrx_2x2.rows);
    free_mtrx(mtrx_1x2.elems, mtrx_1x2.rows);
    free_mtrx(mtrx_2x1.elems, mtrx_2x1.rows);
}
