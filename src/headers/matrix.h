#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "vector.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

int **create_A_matrix(FILE *in, int *nodes, node_t *t, int *connections1);
double **create_L_matrix(int **A_matrix, int *D_vector, int n);
double **create_T_matrix(double *a, double *b, int k);
void print_matrix(double **matrix, int n);
void calculate_coefs(double **L_matrix, double *initial_vec, double *prev_initial_vec, double *alfa_coefs, double *beta_coefs, int n, int i, int k);
double **create_G_matrix(double **T_matrix, int n, int x);
void transpose_matrix(double **matrix, int n);
void copy_matrix(double **src_matrix, double **dest_matrix, int n);
double **multiply_mtx_by_mtx(double **left_matrix, double **right_matrix, int n);
void make_G_rotation(double **G_matrix, double **T_matrix, int i);
void force_zeros(double **matrix, int n, double margin);
void calculate_eigenvalue(double **T_matrix, double **Q_matrix, int n, int i);
void assing_eigen(node_t t, double *eigenvector, int n);
void connections(node_t t, int n, int **A_matrix, int *connections2);
void gain_calculate(node_t t, int **A_matrix, int ngroups, int nodes);
void print_gain(node_t t, int nodes);
void free_matrix(double **matrix, int n);
void free_int_matrix(int **matrix, int n);

#endif
