#include "headers/matrix.h"

#define ITERATIONS 12

int main(int argc, char **argv)
{
	char *file_name = argc > 1 ? argv[1] : "data/graf.csrrg";

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
	printf("Macierz Sasiedztwa\n");

	//macierz diagonalna obliczona na podstawie macierzy sasiedztwa
	double **D_matrix = create_D_matrix(A_matrix, nodes, &D_norm);
	printf("Macierz Diagonalna\n");
	
	//macierz Laplace'a
	double **L_matrix = subtract_matrix(D_matrix, A_matrix, nodes);
	printf("Macierz Laplace'a\n");	

	//wektor poczatkowy	
	double *initial_vec = create_initial_vec(D_matrix, D_norm, nodes);
	printf("Wektor poczatkowy\n");	

	//poprzedni wektor poczatkowy
	double *prev_initial_vec = malloc(sizeof(double) * nodes); //aktualnie pusty		

	//inicjacja tablic wspolczynnikow alfa i beta
	double *alfa_coefs = calloc(nodes, sizeof(double));
	double *beta_coefs = calloc(nodes, sizeof(double));

	//obliczenie wspolczynnikow alfa i beta
	calculate_coefs(L_matrix, initial_vec, prev_initial_vec, alfa_coefs, beta_coefs, nodes, 0, ITERATIONS);
	printf("Wspolczynniki alfa i beta\n");

	//utworzenie macierzy tridiagonalnej na postawie wspolczynnikow
	double **T_matrix = tri_matrix(alfa_coefs, beta_coefs, nodes);
	printf("Macierz tridiagonalna\n\n");

	//wypisanie macierzy tridiagonalnej
	print_matrix(T_matrix, ITERATIONS);

	//dodac obliczanie wartosci wlasnych, stworzyc wektory wlasne, posortowac wartosci wektorow wlasnych, policzyc mediane z wartosci wektorow -> podzial grafu na 2 czesci

	return 0;
}
