// Copyright 2021 nat-s.skv@mail.ru
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <matrix.h>

#define WIDTH_OF_FIELD_FOR_ELEM "12"

int is_mtrx_empty(const Matrix_t *mtrx) {
    return (mtrx->rows == 0u) || ( mtrx->cols == 0u) || (mtrx->elems == NULL);
}

static int **calloc_mtrx(size_t rows, size_t cols) {
    if (!rows || !cols) {
        return NULL;
    }

    int **mtrx = calloc(rows, sizeof(int *));
    if (!mtrx) {
        return NULL;
    }

    for (size_t i = 0; i < rows; ++i) {
        mtrx[i] = calloc(cols, sizeof(int));
        if (!mtrx[i]) {
            free_mtrx(mtrx, i);
            return NULL;
        }
    }
    return mtrx;
}

void free_mtrx(int **mtrx, size_t rows) {
    if (!mtrx) {
        return;
    }

    for (size_t i = 0u; i < rows; ++i) {
        if (mtrx[i]) {
            free(mtrx[i]);
        }
    }
    free(mtrx);
}

Matrix_t load_mtrx(FILE *fin) {
    Matrix_t mtrx = {0u, 0u, NULL};

    if (!fin) {
        return mtrx;
    }

    if (ferror(fin)) {
        return mtrx;
    }

    const int expected_input = 2;
    errno = 0;
    if (fscanf(fin, "%zu%zu", &mtrx.rows, &mtrx.cols) != expected_input || errno == ERANGE) {
        return mtrx;
    }

    if (!mtrx.rows || !mtrx.cols) {
        mtrx.rows = 0u;
        mtrx.cols = 0u;
        return mtrx;
    }

    mtrx.elems = calloc_mtrx(mtrx.rows, mtrx.cols);
    if (!mtrx.elems) {
        return mtrx;
    }

    for (size_t i = 0u; i < mtrx.rows; ++i) {
        for (size_t j = 0u; j < mtrx.cols; ++j) {
            if (fscanf(fin, "%d", &mtrx.elems[i][j]) != 1 || errno == ERANGE) {
                free_mtrx(mtrx.elems, mtrx.rows);
                mtrx.rows = 0u;
                mtrx.cols = 0u;
                return mtrx;
            }
        }
    }
    return mtrx;
}

void upload_mtrx(FILE *fout, Matrix_t mtrx) {
    if (!fout || ferror(fout)) {
        return;
    }

    if (is_mtrx_empty(&mtrx)) {
        return;
    }

    fprintf(fout, "%zu %zu\n", mtrx.rows, mtrx.cols);
    for (size_t row = 0u; row < mtrx.rows; ++row) {
        for (size_t col = 0u; col < mtrx.cols; ++col) {
            fprintf(fout, "%" WIDTH_OF_FIELD_FOR_ELEM "d", mtrx.elems[row][col]);
        }
        fprintf(fout, "\n");
    }
}

Matrix_t multiply(Matrix_t lhs_mtrx, Matrix_t rhs_mtrx) {
    Matrix_t product_mtrx = {0u, 0u, NULL};
    if (is_mtrx_empty(&lhs_mtrx) || is_mtrx_empty(&rhs_mtrx)) {
        return product_mtrx;
    }

    if (lhs_mtrx.cols != rhs_mtrx.rows) {
        return product_mtrx;
    }

    product_mtrx.elems = calloc_mtrx(lhs_mtrx.rows, rhs_mtrx.cols);
    if (!product_mtrx.elems) {
        return product_mtrx;
    }
    product_mtrx.rows = lhs_mtrx.rows;
    product_mtrx.cols = rhs_mtrx.cols;

    for (size_t row = 0u; row < lhs_mtrx.rows; ++row) {
        for (size_t col = 0u; col < rhs_mtrx.cols; ++col) {
            for (size_t i = 0u; i < lhs_mtrx.cols; ++i) {
                product_mtrx.elems[row][col] += lhs_mtrx.elems[row][i] * rhs_mtrx.elems[i][col];
            }
        }
    }
    return product_mtrx;
}
