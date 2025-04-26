#ifndef _GROUPS_H_
#define _GROUPS_H_

#include "matrix.h"

//void ingr(node_t t, int nodes, int ngroups, double *root_val);
void ad_nb_nodes(node_t *t, double **L_matrix, int nodes);
void assign_groups(node_t *t, int **A_matrix, int nodes, int ngroups, double *eigenvector, grupa_g g, double **L_matrix, int max_nodes);
int con_am(node_t *t, int **A_matrix, int nodes, int node);
void find_least_con(node_t *t, int **A_matrix, int nodes, int node, int *best_node, int * low_con);
void find_smallest_con(node_t *t, grupa_g g, int **A_matrix, int nodes, int ngroups, int max_gr_size);
void print_l(que_list *head, int row, int col);
void add_to_end(que_list **head, int con);
void add_to_que(que_list **l_gr, node_t *t, int node, int *D_vector);
void list_gr_con(node_t *t, grupa_g g, int nodes, int ngroups, int max_gr_size, int *D_vector, double *eigenvector, double **L_matrix);
void rm_first(que_list **head);
int is_valid(que_list **head, node_t *t, int gr);
void add_from_que(que_list **l_gr, node_t *t,grupa_g g, int gr, int *D_vector);
int dfs(node_t *t, int node, int gr, int *hbs);
int *check_gr_con(node_t *t, grupa_g g, int ngroups, int nodes);
void con_free_nodes(node_t *t, grupa_g g ,int nodes);
void move_nodes_with_negative_gain(node_t *node, int n, grupa_g group, int max, int min);
#endif

