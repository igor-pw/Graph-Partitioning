#ifndef _GROUPS_H_
#define _GROUPS_H_

#include "matrix.h"

void ingr(node_t t, int nodes, int ngroups, double *root_val);
void assign_groups(node_t t, double **Macierz_s, int nodes, int ngroups, double *eigenvector, int centlen);

#endif

