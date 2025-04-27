#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include "vector.h"
#include "graph.h"
#include "groups.h"
#include "structs.h"

int **create_A_matrix(FILE *in, int *nodes, node_t **t, int *connections1);
double **create_L_matrix(int **A_matrix, int *D_vector, int n);
csr_t create_compresed_matrix(double **L_matrix, int nz, int n);
double **create_T_matrix(double *a, double *b, int k);
void print_matrix(double **matrix, int n);
void calculate_coefs(csr_t L_matrix, double *initial_vec, double *prev_initial_vec, double *alfa_coefs, double *beta_coefs, int n, int i, int k);
double **create_G_matrix(double **T_matrix, int n, int x);
void transpose_matrix(double **matrix, int n);
void copy_matrix(double **src_matrix, double **dest_matrix, int n);
double **multiply_mtx_by_mtx(double **left_matrix, double **right_matrix, int n);
void make_G_rotation(double **G_matrix, double **T_matrix, int i);
void calculate_eigenvalue(double **T_matrix, double **Q_matrix, int n, int i);
void assing_eigen(node_t *t, double *eigenvector, int n);
void gain_calculate(node_t *t, int ngroups, int nodes);
void print_gain(node_t *t, int nodes);
void free_matrix(double **matrix, int n);
void free_int_matrix(int **matrix, int n);
void free_csr(csr_t matrix);

#endif
