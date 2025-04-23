#ifndef _GROUPS_H_
#define _GROUPS_H_

#include "matrix.h"

//void ingr(node_t t, int nodes, int ngroups, double *root_val);
void assign_groups(node_t t, int **A_matrix, int nodes, int ngroups, double *eigenvector, int centlen, grupa_g g, double **L_matrix, int max_nodes);
int con_am(node_t t, int **A_matrix, int nodes, int node);
void find_least_con(node_t t, int **A_matrix, int nodes, int node, int *best_node, int * low_con);
void find_smallest_con(node_t t, grupa_g g, int **A_matrix, int nodes, int ngroups, int max_gr_size);

#endif

