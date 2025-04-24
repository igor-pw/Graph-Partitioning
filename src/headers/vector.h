#ifndef _VECTOR_C_
#define _VECTOR_C_

#include "matrix.h"
#include "graph.h"

int *create_D_vector(int **A_matrix, int n, int *D_norm);
double vec_norm(double *vec, int n);
double *vec_sub(double *a, double *b, int k);
void print_vec(double *vec, int n);
double *multiply_mtx_by_vec(double **matrix, double *vec, int n);
double multiply_vec_by_vec(double *vec, double *vecT, int n);
double *create_initial_vec(int *D_vector, int D_norm, int n);
void subtract_vec(double *vec, double *coef_vec, double coef, int n);
void divide_vec(double *vec, double coef, int n);
void copy_vec(double *src_vec, double *dest_vec, int n);
double find_smallest_eigenvalue(double *vec, int n);
double *calculate_eigenvector(double *vec, double **gradient_matrix, int n, double learning_rate, double momentum, double *velocity, double epsilon_margin, double *epsilon);
void eigen_centyl(double *eigenvector, int n, int k, node_t t, grupa_g g, double **L_matrix);
int compare(const void *a, const void *b);

#endif
