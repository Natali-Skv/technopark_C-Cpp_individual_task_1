// Copyright 2021 nat-s.skv@mail.ru
#include <stdio.h>
#include <matrix.h>

int main() {
    Matrix_t matrix1 = {0u, 0u, NULL};
    if (fscanf_mtrx_sizes(stdin, &matrix1)) {
        perror(__FILE__ ": reading sizes of matrix-1 failed");
        return 0;
    }

    matrix1.elems = calloc_mtrx(matrix1.rows, matrix1.cols);
    if (!matrix1.elems) {
        fprintf(stderr, __FILE__ ": malloc_mtrx() for matrix-1  failed\n");
        return 0;
    }
    if (fscanf_mtrx(stdin, &matrix1)) {
        fprintf(stderr, __FILE__ ": fscanf_mtrx() for matrix-1  failed\n");
        free_mtrx(matrix1.elems, matrix1.rows);
        return 0;
    }
    Matrix_t matrix2 = {0u, 0u, NULL};
    if (fscanf_mtrx_sizes(stdin, &matrix2)) {
        perror(__FILE__ ": reading sizes of matrix-2 failed");
        return 0;
    }
    matrix2.elems = calloc_mtrx(matrix2.rows, matrix2.cols);
    if (!matrix2.elems) {
        fprintf(stderr, __FILE__ ": malloc_mtrx() for matrix-2  failed\n");
        free_mtrx(matrix1.elems, matrix1.rows);
        return 0;
    }
    if (fscanf_mtrx(stdin, &matrix2)) {
        fprintf(stderr, __FILE__ ": fscanf_mtrx() for matrix-2  failed\n");
        free_mtrx(matrix1.elems, matrix1.rows);
        free_mtrx(matrix1.elems, matrix2.rows);
        return 0;
    }

    Matrix_t matrix_product = {0u, 0u, NULL};
    multiply_matrices(&matrix1, &matrix2, &matrix_product);

    fprintf(stdout, "\n");
    fprintf_mtrx(stdout, &matrix1);
    fprintf(stdout, "\n");
    fprintf_mtrx(stdout, &matrix2);
    fprintf(stdout, "\n");
    fprintf_mtrx(stdout, &matrix_product);

    free_mtrx(matrix1.elems, matrix1.rows);
    free_mtrx(matrix2.elems, matrix2.rows);
    free_mtrx(matrix_product.elems, matrix_product.rows);
    printf("%s", PROJECT_PATH);
}
