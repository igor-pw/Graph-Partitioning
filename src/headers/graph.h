#ifndef _GRAPH_H_
#define _GRAPH_H_
#include "matrix.h"

typedef struct node{
	int *connected;
	int nr, x, y, group, index, vle, gain, gr_gain;
	double eigenvalue;
} * node_t;

typedef struct grupa{
	int *gr_nodes;
	int *no_con;
	int gr_size;
} * grupa_g;

//void print_results(node_t t, int nodes, int ngroups, int **Macierz_s, int max_nodes, int low_nodes);

/*
void scan_flags(int argc, char **argc, char *flag_value);
void divide_graph(double **A_matrix, node_t **nodes, int n);
void print_input(double **A_matrix, node_t **nodes, char *file_name ,int n);
*/
#endif
