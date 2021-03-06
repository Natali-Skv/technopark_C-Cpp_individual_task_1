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

        Matrix_t mtrx_lhs = load_mtrx(fin);
        Matrix_t mtrx_rhs = load_mtrx(fin);
        Matrix_t mtrx_product = multiply(mtrx_lhs, mtrx_rhs);

        upload_mtrx(fout, mtrx_product);

        fseek(fout, 0, SEEK_SET);
        CompareFiles(fout, foutExp);

        free_mtrx(mtrx_lhs.elems, mtrx_lhs.rows);
        free_mtrx(mtrx_rhs.elems, mtrx_rhs.rows);
        free_mtrx(mtrx_product.elems, mtrx_product.rows);
        fclose(fin);
        fclose(fout);
        fclose(foutExp);
    }

    void CompareFiles(FILE *f1, FILE *f2) {
        ASSERT_TRUE(f1 && f2);
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
    FixtureMultiply::SetUp(PROJECT_PATH "tests/input_emulator/multiply/2x2_x_2x4/fin",
                           PROJECT_PATH "tests/input_emulator/multiply/2x2_x_2x4/fout",
                           PROJECT_PATH "tests/input_emulator/multiply/2x2_x_2x4/fexp");
}

TEST_F(FixtureMultiply, MULTIPLY_2X2_X_2X7) {
    FixtureMultiply::SetUp(PROJECT_PATH "tests/input_emulator/multiply/2x2_x_2x7/fin",
                           PROJECT_PATH "tests/input_emulator/multiply/2x2_x_2x7/fout",
                           PROJECT_PATH "tests/input_emulator/multiply/2x2_x_2x7/fexp");
}

TEST_F(FixtureMultiply, MULTIPLY_5X6_X_6X7) {
    FixtureMultiply::SetUp(PROJECT_PATH "tests/input_emulator/multiply/5x6_x_6x7/fin",
                           PROJECT_PATH "tests/input_emulator/multiply/5x6_x_6x7/fout",
                           PROJECT_PATH "tests/input_emulator/multiply/5x6_x_6x7/fexp");
}


TEST(MULTIPLY_MATRICES, MULTIPLY_INVALID_ARGS) {
    FILE *fin = fopen(PROJECT_PATH "tests/input_emulator/multiply/invalid_args", "r");
    Matrix_t mtrx_2x2 = load_mtrx(fin);
    Matrix_t mtrx_2x1 = load_mtrx(fin);

    Matrix_t mtrx_2x2_empty = {2u, 2u, NULL};

    Matrix_t mtrx_product = multiply(mtrx_2x2_empty, mtrx_2x2);
    EXPECT_TRUE(is_mtrx_empty(&mtrx_product));

    mtrx_product = multiply(mtrx_2x2, mtrx_2x2_empty);
    EXPECT_TRUE(is_mtrx_empty(&mtrx_product));

    mtrx_product = multiply(mtrx_2x1, mtrx_2x2);
    EXPECT_TRUE(is_mtrx_empty(&mtrx_product));

    fclose(fin);
    free_mtrx(mtrx_2x2.elems, mtrx_2x2.rows);
    free_mtrx(mtrx_2x1.elems, mtrx_2x1.rows);
}
