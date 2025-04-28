#ifndef _GROUPS_H_
#define _GROUPS_H_

#include "matrix.h"
#include "structs.h"

void ad_nb_nodes(node_t *t, double **L_matrix, int nodes);
void add_to_end(que_list **head, int con);
void add_to_que(que_list **l_gr, node_t *t, int node, int *D_vector);
void list_gr_con(node_t *t, grupa_g g, int nodes, int ngroups, int max_gr_size, int *D_vector, double *eigenvector, double **L_matrix);
void rm_first(que_list **head);
int is_valid(que_list **head, node_t *t);
void add_from_que(que_list **l_gr, node_t *t,grupa_g g, int gr, int *D_vector);
//int dfs(node_t *t, int node, int gr, int *hbs);
int *check_gr_con(node_t *t, grupa_g g, int ngroups, int nodes);
void con_free_nodes(node_t *t, grupa_g g ,int nodes);
void refine_groups(node_t *node, int n, grupa_g group, int max, int min);
bool is_in_margin(grupa_g group, int n, int min, int max);
void free_group(grupa_g group, int n);

#endif

