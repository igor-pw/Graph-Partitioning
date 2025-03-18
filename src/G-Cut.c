#include "headers/matrix.h"

int main(int argc, char **argv)
{
	char *file_name = argc > 1 ? argv[1] : "graf.csrrg";

	if(file_name == NULL)
		return 1;
		
	FILE *in = fopen(file_name, "r");

	int n = 0;
	int A_size = 0;
	int D_norm = 0;

	fscanf(in, "%d\n", &n);
	
	if(n == 0)
		return 2;

	double **A_matrix = create_A_matrix(n, in, &A_size);
	double **Diag = diagonal_matrix(A_matrix, A_size, &D_norm);

	double **Sub = subtract_matrix(Diag, A_matrix ,A_size);
	printf("\n");
	print_matrix(Sub, A_size);
	printf("\n");
	double *initial_vec = create_initial_vec(Diag, D_norm, A_size);

	for(int i = 0; i < A_size; i++)
	      printf("%g ", initial_vec[i]);	

	return 0;
}
