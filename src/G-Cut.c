#include "headers/matrix.h"

int main(int argc, char **argv)
{
	char *file_name = argc > 1 ? argv[1] : "graf.csrrg";

	if(file_name == NULL)
		return 1;
		
	FILE *in = fopen(file_name, "r");

	int n = 0;
	int nodes = 0;
	int D_norm = 0;

	fscanf(in, "%d\n", &n);
	
	if(n == 0)
		return 2;

	//macierz sasiedztwa
	double **A_matrix = create_A_matrix(in, &nodes);	
	
	//macierz diagonalna obliczona na podstawie macierzy sasiedztwa
	double **D_matrix = create_D_matrix(A_matrix, nodes, &D_norm);

	//macierz Laplace'a
	double **L_matrix = subtract_matrix(D_matrix, A_matrix, nodes);
	
	//wektor poczatkowy	
	double *initial_vec = create_initial_vec(D_matrix, D_norm, nodes);

	//od linijki 33 do 49 trzeba zrobic rekurencyjna funkcje dodajaca alfa i beta do tablic
	//wektor pomocniczy
	double *vec = multiply_mtx_by_vec(L_matrix, initial_vec, nodes);

	//wspolczynnik i,i macierzy tridiagonalnej
	double alfa = multiply_vec_by_vec(vec, initial_vec, nodes);
	printf("alfa: %g\n", alfa);

	//wektor pomoczniczy staje sie wektorem reszty
	substract_vec(vec, initial_vec, alfa, nodes); //dla bety != 0 odejmujemy tez beta[i]*initial_vec[j] 

	//wspolczynnik i-1,i oraz i,i-1 macierzy tridiagonalnej
	double beta = vec_norm(vec, nodes);
	printf("beta: %g\n", beta);

	//powstaje drugi wektor poczatkowy dla ktorego powtarzane sa wszystkie kroki od 33 linii
	divide_vec(initial_vec, beta, nodes);	//pozniej trzeba skopiowac initial_vec
	print_vec(initial_vec, nodes);

	return 0;
}
