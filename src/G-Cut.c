#include "headers/matrix.h"

int main(int argc, char **argv)
{
	char *file_name = argc > 1 ? argv[1] : "graf.csrrg";

	if(file_name == NULL)
		return 1;
		
	FILE *in = fopen(file_name, "r");

	int n = 0;
	int A_size = 0;

	fscanf(in, "%d\n", &n);
	
	if(n == 0)
		return 2;

	double **A_matrix = create_A_matrix(n, in, &A_size);
	print_matrix(A_matrix, A_size);

	return 0;
}
