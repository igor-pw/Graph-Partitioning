#ifndef _GRAPH_H_
#define _GRAPH_H_
#include <stdio.h>
#include <stdbool.h>
#include "matrix.h"
#include "structs.h"

void print_results(node_t *t, int nodes, int ngroups, int **A_matrix, int n, int all_edges, char *file_name, int group_margin, bool strict);
int dfs(node_t *t, int node, int gr, int *hbs);
int *count_group_nodes(node_t *node, int nodes, int divide, int min_odes, int max_nodes, int *group_margin);
void find_leaves(node_t *node, int n);
void free_node(node_t *t, int n);

#endif
