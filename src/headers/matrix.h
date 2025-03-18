#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double **create_A_matrix(int n, FILE *in, int *nr_nodes);
double **diagonal_matrix(double **matrix, int A_size, int *D_norm);
double **subtract_matrix(double **matrix1, double **matrix2, int A_size);
double *vector_norm(double *v, int size);
double **tri_matrix(double *a, double *b, int k);
double *vec_sub(double *a, double *b, int k);
void print_matrix(double **matrix, int n);
double *multiply_mtx_by_vec(double **matrix, int n, double *vec);
double multiply_vec_by_vec(double *vec, double *vecT, int n);
double *create_initial_vec(double **D_matrix, int D_norm, int n);

#endif
