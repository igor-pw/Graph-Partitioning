#include "headers/matrix.h"
#include "headers/graph.h"
#define ITERATIONS 2000 //to trzeba dostosowac do rozmiaru grafu bo jak bedzie co ma tylko 20 wierzcholkow to nie zadziala



int main(int argc, char **argv)
{
	char *file_name = argc > 1 ? argv[1] : "data/graf.csrrg";

	if(file_name == NULL)
		return 1;
		
	FILE *in = fopen(file_name, "r");

	int n = 0;
	int nodes = 0;
	int D_norm = 0;
	int connections1 =0;
	fscanf(in, "%d\n", &n);
	
	if(n == 0){
		return 2;
	}

	node_t t = NULL;// = malloc(n * sizeof(struct node));
	

	//macierz sasiedztwa
	double **A_matrix = create_A_matrix(in, &nodes, &t, &connections1);
	double **Macierz_s = malloc(sizeof(double*)*nodes);
	for(int i =0; i < nodes; i++)
		Macierz_s[i]=malloc(sizeof(double)*nodes);
	copy_matrix(A_matrix, Macierz_s, nodes);
	printf("Macierz Sasiedztwa\n");
	
	fclose(in);

	//macierz diagonalna obliczona na podstawie macierzy sasiedztwa
	double **D_matrix = create_D_matrix(A_matrix, nodes, &D_norm);
	printf("Macierz Diagonalna\n");
	
	//macierz Laplace'a
	double **L_matrix = subtract_matrix(D_matrix, A_matrix, nodes);
	printf("Macierz Laplace'a\n");	

	free_matrix(A_matrix, nodes);

	//wektor poczatkowy	
	double *initial_vec = create_initial_vec(D_matrix, D_norm, nodes);
	printf("Wektor poczatkowy\n");	

	free_matrix(D_matrix, nodes);
	
	//poprzedni wektor poczatkowy
	double *prev_initial_vec = malloc(sizeof(double) * nodes); //aktualnie pusty		

	//inicjacja tablic wspolczynnikow alfa i beta
	double *alfa_coefs = calloc(nodes, sizeof(double));
	double *beta_coefs = calloc(nodes, sizeof(double));

	//obliczenie wspolczynnikow alfa i beta
	calculate_coefs(L_matrix, initial_vec, prev_initial_vec, alfa_coefs, beta_coefs, nodes, 0, ITERATIONS);
	printf("Wspolczynniki alfa i beta\n");

	free(initial_vec);
	free(prev_initial_vec);

	//utworzenie macierzy tridiagonalnej na postawie wspolczynnikow
	double **T_matrix = tri_matrix(alfa_coefs, beta_coefs, ITERATIONS);
	printf("Macierz Tridiagonalna\n");

	free(alfa_coefs);
	free(beta_coefs);

	//macierz ortogonalna, poczatkowo pusta	
	double **Q_matrix = malloc(sizeof(double*) * ITERATIONS);

	for(int i = 0; i < ITERATIONS; i++)
		Q_matrix[i] = calloc(ITERATIONS, sizeof(double));

	//zamiana macierzy tridiagonalnej w diagonalna, ktorej elementy diagonali sa wartosciami wlasnymi	
	for(int i = 0; i < ITERATIONS/2; i++)
		calculate_eigenvalue(T_matrix, Q_matrix, ITERATIONS, 0);

	free_matrix(Q_matrix, ITERATIONS);


	printf("Wartosci wlasne\n");

	//kopiujemy wartosci wlasne do wektora
	double *eigenvalues_vec = malloc(sizeof(double) * ITERATIONS);

	for(int i = 0; i < ITERATIONS; i++)
		eigenvalues_vec[i] = T_matrix[i][i];

	free_matrix(T_matrix, ITERATIONS);

	//szukana wartosc wlasna
	double eigenvalue = find_smallest_eigenvalue(eigenvalues_vec, ITERATIONS);
	printf("Najmniejsza wartosc wlasna macierzy Laplace'a\n\n");

	free(eigenvalues_vec);

	//wektor wlasny, nie wiem czy jest poprawnie policzony
	double *eigenvector = calculate_eigenvector(eigenvalue, nodes, 0.075);
	printf("Wektor wlasny\n");
	
	assing_eigen(t,eigenvector,nodes);
	/*
	for(int i = 0; i <n;i++){
		for(int j =0; j <n ;j++){
			if(t[i][j].in ==1)
				printf("%3lf ",t[i][j].eigenvalue);
			else
				printf("0    ");
		}
		printf("\n");
	}
	*/

	//sortujemy wektor wlasny
	qsort(eigenvector, nodes, sizeof(double), compare);

	printf("Wartosc wlasna: %g\n", eigenvalue);
	print_vec(eigenvector, nodes);
	
	//obliczamy mediane, narazie tylko dla podzialu na 2 czesc
	double median = calculate_median(eigenvector, 2, nodes);
	printf("Mediana: %lf\n", median);
	assing_group(t,median,nodes);
	int connections2 =0;//to liczy tylko raz czyli z 1 do 2 a nie z 1 do 2 i z 2 do 1
	connections(t,nodes, Macierz_s, &connections2);
	int lu = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(t[lu].y == i && t[lu].x ==j){
				printf("1 ");
				lu++;
			}
			else
				printf("0 ");
		}
		printf("\n");
	}
	int ngroups = 2;

		printf("\n");
	for(int k = 1; k <=ngroups; k++){
		printf("grupa %d\n", k);
		for(int i =0; i<nodes; i++){
			int vle = t[i].vle;
			if( t[i].group == k && vle != 0){
					for(int j = 0; j < vle; j++)
						printf("%d-%d\n",i,t[i].connected[j]);
			}
		}
		printf("\n");

	}

/*	
	lu = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(t[lu].y == i && t[lu].x ==j){
				printf("%d ",t[lu].group);
				lu++;
			}
			else
				printf("0 ");
		}
		printf("\n");
	}

*/

	

	int counter = 0;

	while(eigenvector[counter] < median)
		counter++;

	printf("ilosc wierzcholkow w 1 grupie: %d\nilosc wierzcholkow w 2 grupie: %d\n", counter, nodes-counter);

	printf(" ilosc polanczen przed:\t %d,\n ilosc polonczen po:\t %d, \n ilosc usunietych polonczen:\t %d, \n", connections1,connections2, connections1-connections2);

	//gdzie jeszcze nie jest zwalniana pamiec
	free_matrix(L_matrix, nodes);
	free(eigenvector);

	return 0;
}

