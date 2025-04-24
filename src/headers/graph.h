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

void print_results(node_t *t, int nodes, int ngroups, int **A_matrix, int max_nodes, int low_nodes, int n, int all_edges);
void free_struct_node(node_t *t, int n);

#endif
