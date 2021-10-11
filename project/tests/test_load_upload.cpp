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

        Matrix_t mtrx = load_mtrx(fin);
        upload_mtrx(fout, mtrx);

        fseek(fin, 0, SEEK_SET);
        fseek(fout, 0, SEEK_SET);

        CompareFiles(fin, fout);

        free_mtrx(mtrx.elems, mtrx.rows);
        fclose(fin);
        fclose(fout);
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
    FixturePrintScan::SetUp(PROJECT_PATH "tests/input_emulator/load_upload/load/2x2",
                            PROJECT_PATH "tests/input_emulator/load_upload/upload/2x2");
}

TEST_F(FixturePrintScan, MATRIX_5X1) {
    FixturePrintScan::SetUp(PROJECT_PATH "tests/input_emulator/load_upload/load/5x1",
                            PROJECT_PATH "tests/input_emulator/load_upload/upload/5x1");
}

TEST_F(FixturePrintScan, MATRIX_7X6) {
    FixturePrintScan::SetUp(PROJECT_PATH "tests/input_emulator/load_upload/load/7x6",
                            PROJECT_PATH "tests/input_emulator/load_upload/upload/7x6");
}

TEST_F(FixturePrintScan, MATRIX_1X7) {
    FixturePrintScan::SetUp(PROJECT_PATH "tests/input_emulator/load_upload/load/1x7",
                            PROJECT_PATH "tests/input_emulator/load_upload/upload/1x7");
}

TEST(FSCANF_FPRINTF, ERANGE) {
    FILE *fin = fopen(PROJECT_PATH "tests/input_emulator/load_upload/erange", "r");
    Matrix_t mtrx = load_mtrx(fin);
    EXPECT_TRUE( is_mtrx_empty(&mtrx));
    fclose(fin);
}
