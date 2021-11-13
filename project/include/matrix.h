// Copyright 2021 nat-s.skv@mail.ru
#ifndef PROJECT_INCLUDE_MATRIX_H_
#define PROJECT_INCLUDE_MATRIX_H_

#include <stdio.h>

typedef struct Matrix {
    size_t rows;
    size_t cols;
    int **elems;
} Matrix_t;

int is_mtrx_empty(const Matrix_t *mtrx);

void free_mtrx(int **mtrx, size_t rows);

Matrix_t load_mtrx(FILE *fin);

void upload_mtrx(FILE *fout, Matrix_t mtrx);

Matrix_t multiply(Matrix_t lhs_mtrx, Matrix_t rhs_mtrx);

#endif  // PROJECT_INCLUDE_MATRIX_H_
