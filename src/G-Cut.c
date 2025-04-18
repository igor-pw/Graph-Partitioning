#include "headers/matrix.h"
#include "headers/graph.h"
#include "headers/input.h"
#include <time.h>
#define ITERATIONS 50 //to trzeba dostosowac do rozmiaru grafu bo jak bedzie co ma tylko 20 wierzcholkow to nie zadziala

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
	printf("1\n");
	double **I_matrix = create_I_matrix(nodes, eigenvalue);

	printf("2\n");
	//for(int i = 0; i < nodes; i++)
		//initial_vec[i] = L_matrix[i][i]/sqrt(D_norm); 

	printf("3\n");
	double **gradient_matrix = subtract_matrix(L_matrix, I_matrix, nodes);

	printf("4\n");
	double learning_rate = 0.001;
	double momentum = 0.8;

	printf("5\n");
	if(nodes >= 200)
		learning_rate = 0.01;

	printf("6\n");
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

	printf("7\n");
	double *velocity = calloc(n, sizeof(double));

	printf("8\n");
	//wektor wlasny, nie wiem czy jest poprawnie policzony
	double *eigenvector = calculate_eigenvector(initial_vec, gradient_matrix, nodes, learning_rate, momentum, velocity, epsilon_margin);
	printf("Wektor wlasny\n");
	
	assing_eigen(t,eigenvector,nodes);

	//sortujemy wektor wlasny
	qsort(eigenvector, nodes, sizeof(double), compare);

	int ngroups = 17; // divide; //<------ ILOSC GRUP (tymczasowo)
	
	int centlen = ngroups+1;
	// |Najmniejsza wartosc | pomiedzy z rownymi odstempami|najwieksza wartosc| 
	double *centyle = malloc(centlen * sizeof(double));
	printf("centyle: \n");
	eigen_centyl(centyle, ngroups, eigenvector, nodes); //174

	printf("Wartosc wlasna: %g\n", eigenvalue);
	print_vec(eigenvector, nodes);
	
	//obliczamy mediane, narazie tylko dla podzialu na 2 czesc
	double median = calculate_median(eigenvector, 2, nodes);
	printf("Mediana: %lf\n", median);
	
	assing_group(t,nodes,ngroups,centyle);
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
			int vle = t[i].vle;
			if( t[i].group == k && vle != 0){
					for(int j = 0; j < vle; j++)
						printf("%d-%d\n",i,t[i].connected[j]);
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
		printf("ilosc wieszoclkow w gr %d: %d\n", i , countgr);
	}


	printf(" ilosc polanczen przed:\t %d,\n ilosc polaczen po:\t %d, \n ilosc usunietych polonczen:\t %d, \n", connections1,connections2, connections1-connections2);

	gain_calculate(t, Macierz_s, ngroups, nodes);
	print_gain(t, nodes);

	//gdzies jeszcze nie jest zwalniana pamiec
	free_matrix(L_matrix, nodes);
	free(eigenvector);

	return 0;
}

