// Copyright 2021 nat-s.skv@mail.ru
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <matrix.h>

#define WIDTH_OF_FIELD_FOR_ELEM "12"

int **calloc_mtrx(size_t rows, size_t cols) {
    if (!rows || !cols) {
        fprintf(stderr, __FILE__ ": attempt to allocate 0 bytes\n");
        return NULL;
    }
    int **mtrx = calloc(rows, sizeof(int *));
    if (!mtrx) {
        perror(__FILE__ ": calloc() failed");
        return NULL;
    }
    for (size_t row = 0; row < rows; ++row) {
        mtrx[row] = calloc(cols, sizeof(int));
        if (!mtrx[row]) {
            perror(__FILE__ ": calloc() failed");
            free_mtrx(mtrx, row);
            return NULL;
        }
    }
    return mtrx;
}

void free_mtrx(int **mtrx, size_t rows) {
    if (!mtrx) {
        return;
    }
    for (size_t row = 0; row < rows; ++row) {
        free(mtrx[row]);
    }
    free(mtrx);
}

int fscanf_mtrx_sizes(FILE *fin, Matrix_t *mtrx) {
    if (!fin || !mtrx) {
        fprintf(stderr, __FILE__ ": NULL transmitted as argument to fscanf_mtrx()\n");
        return 1;
    }
    errno = 0;
    if (fscanf(fin, "%zu%zu", &mtrx->rows, &mtrx->cols) != 2) {
        perror(__FILE__ ": reading sizes of matrix failed");
        return 1;
    }
    if (errno == ERANGE) {
        perror(__FILE__ ": reading of matrix sizes failed: size of matrix is too large");
        return 1;
    }
    return 0;
}

int fscanf_mtrx(FILE *fin, Matrix_t *mtrx) {
    if (!fin || !mtrx) {
        fprintf(stderr, __FILE__ ": NULL transmitted as argument\n");
        return 1;
    }
    if (!mtrx->elems) {
        fprintf(stderr, __FILE__ ": matrix is empty\n");
        return 1;
    }
    for (size_t row = 0u; row < mtrx->rows; ++row) {
        for (size_t col = 0u; col < mtrx->cols; ++col) {
            if (fscanf(fin, "%d", &mtrx->elems[row][col]) != 1) {
                perror(__FILE__ ": reading of matrix element  failed");
                return 1;
            }
            if (errno == ERANGE) {
                perror(__FILE__ ": reading of matrix element failed");
                return 1;
            }
        }
    }
    return 0;
}

int fprintf_mtrx(FILE *fout, const Matrix_t *mtrx) {
    if (!fout || !mtrx) {
        fprintf(stderr, __FILE__ ": NULL transmitted as argument to fscanf_mtrx()\n");
        return 1;
    }
    if (!mtrx->cols || !mtrx->rows || !mtrx->elems) {
        fprintf(stderr, __FILE__ ":  matrix is empty\n");
        return 1;
    }
    fprintf(fout, "%zu %zu\n", mtrx->rows, mtrx->cols);
    for (size_t row = 0u; row < mtrx->rows; ++row) {
        for (size_t col = 0u; col < mtrx->cols; ++col) {
            fprintf(fout, "%" WIDTH_OF_FIELD_FOR_ELEM "d", mtrx->elems[row][col]);
        }
        fprintf(fout, "\n");
    }
    return 0;
}

int multiply_matrices(const Matrix_t *lhs_mtrx, const Matrix_t *rhs_mtrx, Matrix_t *product_mtrx) {
    if (!lhs_mtrx || !rhs_mtrx || !product_mtrx) {
        fprintf(stderr, __FILE__ ": NULL transmitted as argument to fscanf_mtrx()\n");
        return 1;
    }
    if (lhs_mtrx == product_mtrx || rhs_mtrx == product_mtrx) {
        fprintf(stderr, __FILE__ ": product-matrix pointer can not be the same as multiplier-matrix pointer\n");
        return 1;
    }
    if (lhs_mtrx->cols != rhs_mtrx->rows) {
        fprintf(stderr, __FILE__ ": multiplication is not defined for matrices lhs_mtrx->cols != rhs_mtrx->rows\n");
        return 1;
    }

    if (!lhs_mtrx->elems || !rhs_mtrx->elems) {
        fprintf(stderr, __FILE__ ": multiplier-matrices can not be empty\n");
        return 1;
    }

    if (product_mtrx->elems) {
        fprintf(stderr, __FILE__ ": product-matrix must be empty\n");
        return 1;
    }
    product_mtrx->rows = lhs_mtrx->rows;
    product_mtrx->cols = rhs_mtrx->cols;
    product_mtrx->elems = calloc_mtrx(product_mtrx->rows, product_mtrx->cols);
    if (!product_mtrx->elems) {
        fprintf(stderr, __FILE__ ": malloc_mtrx() for matrix-product  failed\n");
        product_mtrx->cols = 0u;
        product_mtrx->rows = 0u;
        return 1;
    }
    for (size_t row = 0u; row < lhs_mtrx->rows; ++row) {
        for (size_t col = 0u; col < rhs_mtrx->cols; ++col) {
            for (size_t i = 0u; i < lhs_mtrx->cols; ++i) {
                product_mtrx->elems[row][col] += lhs_mtrx->elems[row][i] * rhs_mtrx->elems[i][col];
            }
        }
    }
    return 0;
}
