#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "vector.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

double **create_A_matrix(FILE *in, int *nodes, node_t *t);
double **create_D_matrix(double **matrix, int A_size, int *D_norm);
double **subtract_matrix(double **matrix1, double **matrix2, int n);
double **tri_matrix(double *a, double *b, int k);
void print_matrix(double **matrix, int n);
void calculate_coefs(double **L_matrix, double *initial_vec, double *prev_initial_vec, double *alfa_coefs, double *beta_coefs, int n, int i, int k);
double **create_G_matrix(double **T_matrix, int n, int x);
void transpose_matrix(double **matrix, int n);
void copy_matrix(double **src_matrix, double **dest_matrix, int n);
double **multiply_mtx_by_mtx(double **left_matrix, double **right_matrix, int n);
void make_G_rotation(double **G_matrix, double **T_matrix, int i);
void force_zeros(double **matrix, int n, double margin);
void calculate_eigenvalue(double **T_matrix, double **Q_matrix, int n, int i);
//double **create_I_matrix(int n, double coef);
void assing_eigen(node_t t, double *eigenvector, int n);
void assing_group(node_t t, double mediana, int n);
void connections(node_t t, int n, double **A_matrix);
void free_matrix(double **matrix, int n);

#endif
