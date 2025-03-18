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
	
	double **Diag = diagonal_matrix(A_matrix, A_size);
	double **Sub = subtract_matrix(Diag, A_matrix ,A_size);
	print_matrix(A_matrix, A_size);
	printf("\n");
	printf("\n");
	printf("\n");
	print_matrix(Diag, A_size);
	printf("\n");
	printf("\n");
	printf("\n");
	print_matrix(Sub, A_size);
	printf("\n");
	printf("\n");
	printf("\n");
	//testowanie funkcji 
	double a[15];
	double b[14];
	double c[14];
	for(int i = 0; i <14; i++){
		a[i] = i;
		b[i] = 2*i;
		c[i] = i ;
	}
	double *uwu = vec_sub(b,c,14);
	a[14] = 15;
	double *norm_a = vector_norm(a,15);
	double *norm_b = vector_norm(b,14);
	
	printf("\n");
	double *initial_vec = create_initial_vec(Diag, D_norm, A_size);

	for(int i = 0; i < A_size; i++)
	      printf("%g ", initial_vec[i]);	

	for( int i =0; i < 14; i ++){
		printf("%lf,	",norm_a[i]);
		printf("%lf,	",uwu[i]);
		printf("%lf\n",norm_b[i]);

	}
	printf("\n");
	printf("\n");
	printf("\n");
	double **tri = tri_matrix(norm_a, norm_b, 15);
	print_matrix(tri, 15);
	
	return 0;
}
