// Copyright 2021 nat-s.skv@mail.ru
extern "C" {
#include <matrix.h>
}

#include <gtest/gtest.h>
#include <cstring>
#include <fstream>


class FixturePrintScan : public ::testing::Test {
protected:
    static const int bufSize = 100;

    void SetUp(const char *finPath, const char *foutPath) {
        FILE *fin = fopen(finPath, "r");
        FILE *fout = fopen(foutPath, "w+");
        ASSERT_TRUE(fin && fout);
        Matrix_t mtrx = {0u, 0u, NULL};
        EXPECT_EQ(fscanf_mtrx_sizes(fin, &mtrx), 0);
        mtrx.elems = calloc_mtrx(mtrx.rows, mtrx.cols);
        ASSERT_TRUE(mtrx.elems || !mtrx.rows || !mtrx.cols);
        EXPECT_EQ(fscanf_mtrx(fin, &mtrx) * mtrx.rows * mtrx.cols, 0);
        EXPECT_EQ(fprintf_mtrx(fout, &mtrx), 0);
        fseek(fin, 0, SEEK_SET);
        fseek(fout, 0, SEEK_SET);
        CompareFiles(fin, fout);
        free_mtrx(mtrx.elems, mtrx.rows);
        EXPECT_EQ(fclose(fin), 0);
        EXPECT_EQ(fclose(fout), 0);
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

    void TearDown() override {
    }
};

TEST_F(FixturePrintScan, MATRIX_2X2) {
    FixturePrintScan::SetUp(PROJECT_PATH "tests/input_emulator/fscanf_fprintf/fscanf/2x2",
                            PROJECT_PATH "tests/input_emulator/fscanf_fprintf/fprintf/2x2");
}

TEST_F(FixturePrintScan, MATRIX_5X1) {
    FixturePrintScan::SetUp(PROJECT_PATH "tests/input_emulator/fscanf_fprintf/fscanf/5x1",
                            PROJECT_PATH "tests/input_emulator/fscanf_fprintf/fprintf/5x1");
}

TEST_F(FixturePrintScan, MATRIX_7X6) {
    FixturePrintScan::SetUp(PROJECT_PATH "tests/input_emulator/fscanf_fprintf/fscanf/7x6",
                            PROJECT_PATH "tests/input_emulator/fscanf_fprintf/fprintf/7x6");
}

TEST_F(FixturePrintScan, MATRIX_1X7) {
    FixturePrintScan::SetUp(PROJECT_PATH "tests/input_emulator/fscanf_fprintf/fscanf/1x7",
                            PROJECT_PATH "tests/input_emulator/fscanf_fprintf/fprintf/1x7");
}

TEST_F(FixturePrintScan, MATRIX_0X7) {
    FixturePrintScan::SetUp(PROJECT_PATH "tests/input_emulator/fscanf_fprintf/fscanf/0x7",
                            PROJECT_PATH "tests/input_emulator/fscanf_fprintf/fprintf/0x7");
}

TEST(FSCANF_FPRINTF, NULL_ARGS) {
    Matrix_t mtrx = {0u, 0u, NULL};
    EXPECT_NE(fscanf_mtrx_sizes(NULL, &mtrx), 0);
    EXPECT_NE(fscanf_mtrx_sizes(stdin, NULL), 0);
    EXPECT_NE(fscanf_mtrx(NULL, &mtrx), 0);
    EXPECT_NE(fscanf_mtrx(stdin, NULL), 0);
    EXPECT_NE(fscanf_mtrx(stdin, &mtrx), 0);
}
