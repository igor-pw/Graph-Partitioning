#include "headers/matrix.h"
#include "headers/graph.h"
#include "headers/input.h"
#include "headers/groups.h"
#include <time.h>
//#define ITERATIONS 50 //to trzeba dostosowac do rozmiaru grafu bo jak bedzie co ma tylko 20 wierzcholkow to nie zadziala

int main(int argc, char **argv)
{
	//srand(time(NULL));
	Flags flags = Error;
	int argv_index = 1;
	char *flag_value = NULL;
	int divide = 2;
	double margin = 0.1;
	char *input_name = NULL;
	char *output_name = "output.txt";

	for(int i = 0; i < (argc-1)/2; i++)
	{
		void *ptr_flag_value = scan_flags(&flags, argv, argv_index);
	
		flag_value = (char*)ptr_flag_value;

		if(flags == 1)
			divide = atoi(flag_value);

		else if(flags == 2)
			margin = atof(flag_value);

		else if(flags == 3)
			input_name = flag_value;
	
		else if(flags == 4)
			output_name = flag_value;
	
		argv_index += 2;
	}

	printf("%d, %g, %s, %s\n", divide, margin, input_name, output_name);

	if(input_name == NULL)
		return 1;
		
	FILE *in = fopen(input_name, "r");

	int n = 0;
	int nodes = 0;
	int D_norm = 0;
	int connections1 =0;
	fscanf(in, "%d\n", &n);
	
	if(n == 0){
		return 2;
	}
	divide = 17;
	//margin = 0.01;
	node_t t = NULL;// = malloc(n * sizeof(struct node));
	grupa_g g = malloc(divide * sizeof(struct grupa));
	
	

	//macierz sasiedztwa
	double **A_matrix = create_A_matrix(in, &nodes, &t, &connections1);
	double **Macierz_s = malloc(sizeof(double*)*nodes);
	for(int i =0; i < nodes; i++)
		Macierz_s[i]=malloc(sizeof(double)*nodes);
	copy_matrix(A_matrix, Macierz_s, nodes);
	printf("Macierz Sasiedztwa\n");
	
	fclose(in);

	int ITERATIONS;

	if(nodes <= 50)
		ITERATIONS = nodes-1;
	else
		ITERATIONS = 50;
	
	// ilosc wieszcholkow w grupach
	int avg_nodes = (int)round((double)nodes/divide); // srednia ilosc wieszcholkow
	int max_nodes = (int)floor((double)nodes/divide*(1+margin)); // to ile moze byc max w grupie z marginesem
	int low_nodes = (int)ceil((double)nodes/divide*(1-margin)); // to ile moze byc najmniej w grupie z marginesem
	// zrobilem to w to bo np avg moze byc 1.4 i jak zaokrogle to margines moze sie rozjechac

	if(max_nodes*divide < nodes){
		printf("podzial na %d czensci przy marginesie %lf nie jest mozliwy\n",divide,margin);
		printf("prosze zmienic ilosc grup na jaka chcemy podzielic graf, badz zwiekszyc margines!\n");
		return 1;
	}

	for(int i = 0; i < divide; i++){
		g[i].gr_nodes = malloc(nodes * sizeof(double)); // dalem tak bo potem moze przez chwile bedzie potrzebne zeby poza margines wychodzilo
	}	

	//macierz diagonalna obliczona na podstawie macierzy sasiedztwa
	double **D_matrix = create_D_matrix(A_matrix, nodes, &D_norm);
	printf("Macierz Diagonalna\n");
	
	//macierz Laplace'a
	double **L_matrix = subtract_matrix(D_matrix, A_matrix, nodes);
	printf("Macierz Laplace'a\n");
	double **Macierz_L = malloc(sizeof(double*)*nodes);
	for(int i =0; i < nodes; i++)
		Macierz_L[i]=malloc(sizeof(double)*nodes);
	copy_matrix(L_matrix, Macierz_L, nodes);
	


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

	//free(initial_vec);
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
	double **I_matrix = create_I_matrix(nodes, eigenvalue);

	double **gradient_matrix = subtract_matrix(L_matrix, I_matrix, nodes);

	double learning_rate = 0.001;
	double momentum = 0.8;

	if(nodes >= 200)
		learning_rate = 0.01;

	double epsilon_margin;

	if(nodes <= 200)
		epsilon_margin = pow(10, -12);
	else if(nodes > 200 && nodes <= 500)
		epsilon_margin = pow(10, -8);
	else if(nodes > 500 && nodes <= 1000)
		epsilon_margin = pow(10, -6);
	else if(nodes > 1000 && nodes <= 10000)
		epsilon_margin = pow(10, -4);
	else
		epsilon_margin = pow(10, -3);

	double *velocity = calloc(nodes, sizeof(double));

	//wektor wlasny, nie wiem czy jest poprawnie policzony
	double *eigenvector = calculate_eigenvector(initial_vec, gradient_matrix, nodes, learning_rate, momentum, velocity, epsilon_margin);
	printf("Wektor wlasny\n");
	
	assing_eigen(t,eigenvector,nodes);

	//sortujemy wektor wlasny
	qsort(eigenvector, nodes, sizeof(double), compare);

	int ngroups = divide;//////      56 linijka	 // divide; //<------ ILOSC GRUP (tymczasowo)
	
	int centlen = ngroups+1;

	printf("Wartosc wlasna: %g\n", eigenvalue);
	print_vec(eigenvector, nodes);
	
	//obliczamy mediane, narazie tylko dla podzialu na 2 czesc
	double median = calculate_median(eigenvector, 2, nodes);
	printf("Mediana: %lf\n", median);
	
	printf("Przydzielanie grup\n");
	assign_groups(t, Macierz_s, nodes, ngroups, eigenvector, centlen, g, Macierz_L, max_nodes);
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

		printf("\n");
	for(int k = 0; k <ngroups; k++){
		printf("grupa %d\n", k);
		for(int i =0; i<nodes; i++){
			if( t[i].group == k){
					for(int j = i; j < nodes; j++){
						if(t[j].group == k && Macierz_s[i][j] == 1)
							printf("%d-%d\n",i,j);
					}
			}
		}
		printf("\n");

	}

	int counter = 0;

	while(eigenvector[counter] < median)
		counter++;
	printf("ilosc wszystkich wieszcholkow: %d\n", nodes);
	for(int i = 0; i < ngroups; i ++){
		int countgr = 0;
		for(int j = 0; j < nodes; j ++){
			if(t[j].group == i){
				countgr++;
			}
		}
		printf("ilosc wieszoclkow w gr %d: %d", i , countgr);
		if(countgr <low_nodes || countgr > max_nodes)
			printf(" <---- grupa niezgonda z marginesem!!!!\n");
		else
			printf("\n");
	}
	int wolne_wieszcholki = 0;
	for(int i =0; i< nodes; i++){
		if(t[i].group == -1)
			wolne_wieszcholki++;
	}
	printf("ilosc wolnych wieszcholkow: %d\n",wolne_wieszcholki);




	gain_calculate(t, Macierz_s, ngroups, nodes);
	print_gain(t, nodes);

	//gdzies jeszcze nie jest zwalniana pamiec
	free_matrix(A_matrix, nodes);
	free_matrix(L_matrix, nodes);
	free_matrix(Macierz_s, nodes);
	free_matrix(Macierz_L, nodes);
	free(eigenvector);

	return 0;
}

