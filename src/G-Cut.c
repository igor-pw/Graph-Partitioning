#include "headers/matrix.h"
#include "headers/graph.h"
#include "headers/input.h"
#include "headers/groups.h"
#include "headers/vector.h"

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
	node_t *t = NULL;
	grupa_g g = malloc(divide * sizeof(struct grupa));
	
	//macierz sasiedztwa
	int **A_matrix = create_A_matrix(in, &nodes, &t, &connections1);
	printf("Macierz Sasiedztwa\n");

	fclose(in);

	int ITERATIONS = 50;

	if(nodes <= 50)
		ITERATIONS = nodes;
	
	// ilosc wierzcholkow w grupach
	int avg_nodes = (int)round((double)nodes/divide); // srednia ilosc wierzcholkow
	int max_nodes = (int)floor((double)nodes/divide*(1+margin)); // to ile moze byc max w grupie z marginesem
	int low_nodes = (int)ceil((double)nodes/divide*(1-margin)); // to ile moze byc najmniej w grupie z marginesem
	// zrobilem to w to bo np avg moze byc 1.4 i jak zaokrogle to margines moze sie rozjechac

	//cos tu nie gra bo dla marginesu 1% max jest mniejszy od low
	printf("nodes: %d, avg: %d, max: %d, low: %d, avg*nodes: %d\n", nodes, avg_nodes, max_nodes, low_nodes, avg_nodes*divide);

	//noi ten warunek nie wiem dlaczego tez nie dziala bo przechodzi dla 105 < 102 co nie jest prawda
	if(max_nodes*divide < nodes){		
		printf("podzial na %d czensci przy marginesie %lf nie jest mozliwy\n",divide,margin);
		printf("prosze zmienic ilosc grup na jaka chcemy podzielic graf, badz zwiekszyc margines!\n");
		return 1;
	}

	for(int i = 0; i < divide; i++){
		g[i].gr_nodes = malloc(nodes * sizeof(int)); // dalem tak bo potem moze przez chwile bedzie potrzebne zeby poza margines wychodzilo
		g[i].no_con = calloc(nodes, sizeof(int));
	}	

	//wektor stopni obliczony na podstawie macierzy sasiedztwa
	//zmienilem na wektor zeby mniej pamieci zajmowalo
	int *D_vector = create_D_vector(A_matrix, nodes, &D_norm);
	printf("Wektor Stopni\n");

	int all_edges = 0;
	int nz = 0;

	for(int i = 0; i < nodes; i++)
	{
		all_edges += D_vector[i];
		nz += (D_vector[i] + 1);
	}

	all_edges /= 2;

	//macierz Laplace'a
	double **L_matrix = create_L_matrix(A_matrix, D_vector, nodes);
	printf("Macierz Laplace'a\n");

	ad_nb_nodes(t,L_matrix,nodes);
	printf("Dodanie sasiadow\n");

	//wektor poczatkowy	
	double *initial_vec = create_initial_vec(D_vector, D_norm, nodes);
	printf("Wektor poczatkowy\n");	

	//poprzedni wektor poczatkowy
	double *prev_initial_vec = malloc(sizeof(double) * nodes); //aktualnie pusty		

	//inicjacja tablic wspolczynnikow alfa i beta
	double *alfa_coefs = calloc(nodes, sizeof(double));
	double *beta_coefs = calloc(nodes, sizeof(double));

	csr_t compresed_L_matrix = create_compresed_matrix(L_matrix, nz, nodes);
	
	//obliczenie wspolczynnikow alfa i beta
	calculate_coefs(compresed_L_matrix, initial_vec, prev_initial_vec, alfa_coefs, beta_coefs, nodes, 0, ITERATIONS);
	printf("Wspolczynniki alfa i beta\n");

	free(prev_initial_vec);
	free_csr(compresed_L_matrix);

	//utworzenie macierzy tridiagonalnej na postawie wspolczynnikow
	double **T_matrix = create_T_matrix(alfa_coefs, beta_coefs, ITERATIONS);
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

	printf("eigenvalue: %g\n", eigenvalue);
	
	free(eigenvalues_vec);

	//tymczasowe przeksztalcenie macierzy Laplace'a w macierz Gradientu
	for(int i = 0; i < nodes; i++)
		L_matrix[i][i] -= eigenvalue;	

	compresed_L_matrix = create_compresed_matrix(L_matrix, nz, nodes);
	
	double learning_rate = 0.001;
	double momentum = 0.8;

	double epsilon_margin = pow(10, -15);

	if(nodes > 5000)
	       epsilon_margin = pow(10, -5);

	double *velocity = calloc(nodes, sizeof(double));

	double epsilon = 0.0;
	double *eigenvector = NULL;

	//wektor wlasny
	do
	{
		eigenvector = calculate_eigenvector(initial_vec, L_matrix, compresed_L_matrix, nodes, learning_rate, momentum, velocity, epsilon_margin, &epsilon);
	
	} while(epsilon > epsilon_margin);

	free(velocity);
	free_csr(compresed_L_matrix);

	//cofniecie zmian w macierzy Laplace'a
	for(int i = 0; i < nodes; i++)
		L_matrix[i][i] += eigenvalue;	

	printf("epsilon: %g\n", epsilon);
	printf("Wektor wlasny\n");
	
	assing_eigen(t, eigenvector,nodes);

	printf("Przydzielenie wartosci wektora wlasnego\n");

	//sortujemy wektor wlasny
	qsort(eigenvector, nodes, sizeof(double), compare);

	int ngroups = divide;
	

	//assign_groups(t, A_matrix, nodes, ngroups, eigenvector, g, L_matrix, max_nodes);
	printf("Przydzielenie grup\n");
	
	list_gr_con(t,g,nodes,ngroups,max_nodes,D_vector,eigenvector,L_matrix);	

	printf("Liscie: \n");
	for(int i = 0; i < nodes; i++)
	{
		if(t[i]->is_leaf)
			printf("wierzcholek: %d\n", t[i]->nr);

	}

	gain_calculate(t, ngroups, nodes);

	for(int i = 0; i < 10; i++)	
	{
		find_leaves(t, nodes);
		move_nodes_with_negative_gain(t, nodes, g, max_nodes, low_nodes);
		gain_calculate(t, ngroups, nodes);
	}
	
	print_results(t,g,nodes,ngroups,A_matrix,max_nodes,low_nodes,n, all_edges);	
	//print_gain(t, nodes);
	printf("finito\n");
	//zwalnianie pamieci (niektore mozna zwolnic wczesniej)
	free_int_matrix(A_matrix, nodes);
	free_matrix(L_matrix, nodes);
	free(eigenvector);
	free(D_vector);
	for(int i = 0; i < divide; i++){
                free(g[i].gr_nodes);
                free(g[i].no_con);
        } 	
	
	free_struct_node(t, nodes);
	free(g);

	return 0;
}

