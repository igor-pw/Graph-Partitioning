#include "matrix.h"

double vec_norm(double *vec, int n);
double *vec_sub(double *a, double *b, int k);
void print_vec(double *vec, int n);
double *multiply_mtx_by_vec(double **matrix, double *vec, int n);
double multiply_vec_by_vec(double *vec, double *vecT, int n);
double *create_initial_vec(double **D_matrix, int D_norm, int n);
void subtract_vec(double *vec, double *coef_vec, double coef, int n);
void divide_vec(double *vec, double coef, int n);
void copy_vec(double *src_vec, double *dest_vec, int n);
double find_smallest_eigenvalue(double *vec, int n);
double *calculate_eigenvector(double *vec, double **gradient_matrix, int n, double learning_rate, double momentum, double *velocity);
double calculate_median(double *eigenvector, int groups, int n); 
void eigen_centyl(double *centyle, int n, double *v, int k);
int compare(const void *a, const void *b);
