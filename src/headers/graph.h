#ifndef _GRAPH_H_
#define _GRAPH_H_
#include "matrix.h"
#include <stdbool.h>

typedef struct csr
{
	double *values;
	int *col_index;
	int *row_ptr;
} *csr_t;

typedef struct node{
	int *connected;
	int nr, x, y, group, index, gain, gr_gain, con_count;
	bool is_leaf;
	double eigenvalue;
} * node_t;

typedef struct grupa{
	int *gr_nodes;
	int *no_con;
	int gr_size;
	int max_con;
} * grupa_g;

typedef struct que_list{
	int que;
	struct que_list *next;
	struct que_list *last;
} que_list;

void print_results(node_t *t,grupa_g g, int nodes, int ngroups, int **A_matrix, int max_nodes, int low_nodes, int n, int all_edges);
void find_leaves(node_t *node, int n);
void free_struct_node(node_t *t, int n);

#endif
