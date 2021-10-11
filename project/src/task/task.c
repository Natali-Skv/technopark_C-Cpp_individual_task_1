// Copyright 2021 nat-s.skv@mail.ru
#include <stdio.h>
#include <matrix.h>
#include <task.h>


int run(FILE *fin, FILE *fout) {
    if (!fin || !fout) {
        return 0;
    }
    if (ferror(fin) || ferror(fout)) {
        fprintf(stderr, __FILE__ ": error exists in the transmitted file\n");
        return 0;
    }

    Matrix_t mtrx_lhs = load_mtrx(fin);
    if (is_mtrx_empty(&mtrx_lhs)) {
        fprintf(stderr, __FILE__ ": load_mtrx() failed\n");
        return 0;
    }

    Matrix_t mtrx_rhs = load_mtrx(fin);
    if (is_mtrx_empty(&mtrx_rhs)) {
        fprintf(stderr, __FILE__ ": load_mtrx() failed\n");
        free_mtrx(mtrx_lhs.elems, mtrx_lhs.rows);
        return 0;
    }

    Matrix_t matrix_product = multiply(mtrx_lhs, mtrx_rhs);

    if (is_mtrx_empty(&matrix_product)) {
        free_mtrx(mtrx_lhs.elems, mtrx_lhs.rows);
        free_mtrx(mtrx_rhs.elems, mtrx_rhs.rows);
        return 0;
    }

    upload_mtrx(fout, mtrx_lhs);
    fprintf(fout, "X\n");
    upload_mtrx(fout, mtrx_rhs);
    fprintf(fout, "=\n");
    upload_mtrx(fout, matrix_product);

    free_mtrx(mtrx_lhs.elems, mtrx_lhs.rows);
    free_mtrx(mtrx_rhs.elems, mtrx_rhs.rows);
    free_mtrx(matrix_product.elems, matrix_product.rows);
    printf("%s", PROJECT_PATH);
    return 0;
}
