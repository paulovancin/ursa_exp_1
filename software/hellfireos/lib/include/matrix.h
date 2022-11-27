#ifndef __MATRIX_H
#define __MATRIX_H
#include <hellfire.h>
#include <fixed.h>
#define NMAX 12


struct matrix {
  int row;
  int column;
  fixed_t str[NMAX][NMAX];
};

struct ekf_out {
  struct matrix states;
  struct matrix MatP;
};


struct matrix set_values(int r, int c, fixed_t s[]);
void print_matrix(struct matrix m);
int get_row(struct matrix m);
int get_column(struct matrix m);
fixed_t get_value(struct matrix m, int row, int col);
struct matrix transposed(struct matrix m);
struct matrix sum(struct matrix m1, struct matrix m2);
struct matrix subtraction(struct matrix m1, struct matrix m2);
struct matrix multiplication(struct matrix m1, struct matrix m2);
struct matrix multE(struct matrix m, fixed_t cte);
struct matrix divE(struct matrix m, fixed_t cte);
struct matrix sumE(struct matrix m, fixed_t cte);
fixed_t determinant(struct matrix m, int order);
struct matrix cofactor(struct matrix m, int rw, int col, int order);
fixed_t auxDet(struct matrix m, int order);
struct matrix inverse(struct matrix m);
struct matrix setEye(int order);
struct matrix zeros(int row, int col);
struct matrix ones(int row, int col);
struct matrix crossProduct3(fixed_t x, fixed_t y, fixed_t z);
struct matrix blkdiag2(struct matrix m1, struct matrix m2);
struct matrix blkdiag3(struct matrix m1, struct matrix m2, struct matrix m3);
struct matrix blkdiag4(struct matrix m1, struct matrix m2, struct matrix m3, struct matrix m4);
struct matrix customMat(int n_mats, int conf[], ...);
struct matrix copy_matrix(struct matrix m);
int rank(struct matrix m);
int checkSymmetry(struct matrix m);
void luDecomposition(struct matrix m, struct matrix* L, struct matrix* U);
void qr(struct matrix m, struct matrix* Q, struct matrix* R);
struct matrix diag(struct matrix m);
struct matrix get_part(int ri, int rf, int ci, int cf, struct matrix m);
struct matrix triu(struct matrix m);
struct matrix tril(struct matrix m);
void householder(struct matrix m, struct matrix* H, struct matrix* Q);
struct matrix eig(struct matrix m);
fixed_t trace(struct matrix m);
struct matrix eig22(struct matrix m);
struct matrix four_by_four_invert(struct matrix in);


#endif
