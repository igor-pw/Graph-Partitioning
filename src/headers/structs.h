#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdbool.h>

typedef struct csr
{
	double *values;
	int *col_index;
	int *row_ptr;
} *csr_t;

typedef struct node{
	int *connected;
	int x, y, group, gain, gr_gain, con_count;
	bool is_leaf;
	bool moved;
	double eigenvalue;
} *node_t;

typedef struct grupa{
	int *gr_nodes;
	int *no_con;
	int gr_size;
	int max_con;
} *grupa_g;

typedef struct que_list{
	int que;
	struct que_list *next;
	struct que_list *last;
} que_list;

#endif
