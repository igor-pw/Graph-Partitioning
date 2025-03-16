#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double **create_A_matrix(int n, FILE *in, int *nr_nodes);
void print_matrix(double **matrix, int n);

#endif
