#include "headers/matrix.h"

#define ITERATIONS 50

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
	double **T_matrix = tri_matrix(alfa_coefs, beta_coefs, ITERATIONS);
	printf("Macierz Tridiagonalna\n");


	//macierz ortogonalna, poczatkowo pusta	
	double **Q_matrix = malloc(sizeof(double*) * ITERATIONS);

	for(int i = 0; i < ITERATIONS; i++)
		Q_matrix[i] = calloc(ITERATIONS, sizeof(double));

	//zamiana macierzy tridiagonalnej w diagonalna, ktorej elementy diagonali sa wartosciami wlasnymi	
	for(int i = 0; i < ITERATIONS/2; i++)
		calculate_eigenvalue(T_matrix, Q_matrix, ITERATIONS, 0);

	printf("Wartosci wlasne\n");
	//wypisanie macierzy tridiagonalnej
	//print_matrix(T_matrix, ITERATIONS);

	//kopiujemy wartosci wlasne do wektora
	double *eigenvalues_vec = malloc(sizeof(double) * ITERATIONS);

	for(int i = 0; i < ITERATIONS; i++)
		eigenvalues_vec[i] = T_matrix[i][i];

	print_vec(eigenvalues_vec, ITERATIONS);

	//znalezc najmniejsza dodatnia wartosc wlana, stworzyc wektory wlasne, posortowac wartosci wektorow wlasnych, policzyc mediane z wartosci wektorow -> podzial grafu na 2 czesci

	return 0;
}
