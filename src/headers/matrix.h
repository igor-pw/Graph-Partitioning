#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

double **create_A_matrix(FILE *in, int *nodes);
double **create_D_matrix(double **matrix, int A_size, int *D_norm);
double **subtract_matrix(double **matrix1, double **matrix2, int n);
double vec_norm(double *vec, int n);
double **tri_matrix(double *a, double *b, int k);
double *vec_sub(double *a, double *b, int k);
void print_matrix(double **matrix, int n);
void print_vec(double *vec, int n);
double *multiply_mtx_by_vec(double **matrix, double *vec, int n);
double multiply_vec_by_vec(double *vec, double *vecT, int n);
double *create_initial_vec(double **D_matrix, int D_norm, int n);
void subtract_vec(double *vec, double *coef_vec, double coef, int n);
void divide_vec(double *vec, double coef, int n);
void copy_vec(double *src_vec, double *dest_vec, int n);
void calculate_coefs(double **L_matrix, double *initial_vec, double *prev_initial_vec, double *alfa_coefs, double *beta_coefs, int n, int i, int k);

#endif
