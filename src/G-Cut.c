#include "headers/matrix.h"
#include "headers/graph.h"
#include "headers/input.h"
#include "headers/groups.h"
#include "headers/vector.h"
#include "headers/structs.h"

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

	if(input_name == NULL)
		return 1;
		
	FILE *in = fopen(input_name, "r");

	if(in == NULL)
	{
		printf("Nieprawidlowa nazwa pliku\n");
		return 2;
	}

	int n = 0;
	int nodes = 0;
	int D_norm = 0;
	int connections1 =0;
	fscanf(in, "%d\n", &n);
	
	if(n == 0)
		return 3;

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
	int max_nodes = (int)floor((double)nodes/divide*(1+margin)); 	
	int low_nodes = (int)ceil((double)nodes/divide*(1-margin)); 	
	printf("nodes: %d, max: %d, low: %d\n", nodes, max_nodes, low_nodes);

	if(max_nodes*divide < nodes){		
		printf("podzial na %d czensci przy marginesie %lf nie jest mozliwy\n",divide,margin);
		printf("prosze zmienic ilosc grup na jaka chcemy podzielic graf, badz zwiekszyc margines!\n");
		return 1;
	}

	for(int i = 0; i < divide; i++){
		g[i].gr_nodes = malloc(nodes * sizeof(int)); 
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
	printf("Najmniejsza wartosc wlasna macierzy Laplace'a\n");

	free(eigenvalues_vec);

	//tymczasowe przeksztalcenie macierzy Laplace'a w macierz Gradientu
	for(int i = 0; i < nodes; i++)
		L_matrix[i][i] -= eigenvalue;	

	compresed_L_matrix = create_compresed_matrix(L_matrix, nz, nodes);
	
	double learning_rate = 0.001;
	double momentum = 0.8;

	double epsilon_margin = pow(10, -12);

	if(nodes > 5000)
	       epsilon_margin = pow(10, -5);

	double *velocity = calloc(nodes, sizeof(double));

	double epsilon = 0.0;
	double prev_epsilon = 0.0;
	double *eigenvector = NULL;

	//wektor wlasny
	do
	{
		eigenvector = calculate_eigenvector(initial_vec, L_matrix, compresed_L_matrix, nodes, learning_rate, momentum, velocity, &epsilon_margin, &epsilon, &prev_epsilon);
	
	} while(epsilon > epsilon_margin);

	free(velocity);
	free_csr(compresed_L_matrix);
	printf("Wektor wlasny\n");

	//cofniecie zmian w macierzy Laplace'a
	for(int i = 0; i < nodes; i++)
		L_matrix[i][i] += eigenvalue;	

	assing_eigen(t, eigenvector,nodes);
	printf("Przydzielenie wartosci wektora wlasnego\n");

	//sortujemy wektor wlasny
	qsort(eigenvector, nodes, sizeof(double), compare);

	int ngroups = divide;
	
	
	list_gr_con(t,g,nodes,ngroups,max_nodes,D_vector,eigenvector,L_matrix);	
	printf("Przydzielenie grup\n");

	gain_calculate(t, ngroups, nodes);
	printf("Obliczenie korzysci dla rafinacji\n");

	for(int i = 0; i < 10; i++)	
	{
		find_leaves(t, nodes);
		refine_groups(t, nodes, g, max_nodes, low_nodes);
		gain_calculate(t, ngroups, nodes);
	}
	printf("Rafinacja grup\n");

	bool in_margin = is_in_margin(g, divide, low_nodes, max_nodes);

	if(in_margin)
		print_results(t,g,nodes,ngroups,A_matrix,max_nodes,low_nodes,n, all_edges, output_name);	
	
	else
		printf("\nPodzial grafu zakonczyl sie niepowodzeniem. Zwieksz margines bledu lub zmniejsz ilosc grup\n");

	//zwolnienie pamieci
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

	if(!in_margin)
		return 4;

	return 0;
}

