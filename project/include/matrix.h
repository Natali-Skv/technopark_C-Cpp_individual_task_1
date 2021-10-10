// Copyright 2021 nat-s.skv@mail.ru
#ifndef PROJECT_INCLUDE_MATRIX_H_
#define PROJECT_INCLUDE_MATRIX_H_

#include <stdio.h>

typedef struct Matrix {
    size_t rows;
    size_t cols;
    int **elems;
} Matrix_t;

int **calloc_mtrx(size_t rows, size_t cols);

void free_mtrx(int **mtrx, size_t rows);

int fscanf_mtrx_sizes(FILE *fin, Matrix_t *mtrx);

int fscanf_mtrx(FILE *fin, Matrix_t *mtrx);

int fprintf_mtrx(FILE *fout, const Matrix_t *mtrx);

int multiply_matrices(const Matrix_t *lhs_mtrx, const Matrix_t *rhs_mtrx, Matrix_t *product_mtrx);

#endif  // PROJECT_INCLUDE_MATRIX_H_
