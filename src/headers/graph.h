#include "matrix.h"

typedef struct node
{
	int nr, x, y, group, index;
	double eigenvalue;
} node_t*;

void scan_flags(int argc, char **argc, char *flag_value);
void divide_graph(double **A_matrix, node_t **nodes, int n);
void print_input(double **A_matrix, node_t **nodes, char *file_name ,int n);
