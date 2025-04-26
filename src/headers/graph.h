#ifndef _GRAPH_H_
#define _GRAPH_H_
#include <stdbool.h>
#include "matrix.h"
#include "structs.h"

void print_results(node_t *t,grupa_g g, int nodes, int ngroups, int **A_matrix, int max_nodes, int low_nodes, int n, int all_edges, char *file_name);
void find_leaves(node_t *node, int n);
void cut_dfs(node_t *node, grupa_g group, int group_nr, int index, int cut_target, int *cut_index, int counter);
void free_struct_node(node_t *t, int n);

#endif
