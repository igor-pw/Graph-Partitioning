#include "headers/matrix.h"
#include "headers/graph.h"
#include "headers/input.h"
#include "headers/groups.h"
#include "headers/vector.h"
#include "headers/structs.h"

int main(int argc, char **argv)
{
	Flags flags = Error;
	int argv_index = 1;
	char *flag_value = NULL;
	
	//domyslne wartosci flag 
	int divide = 2;
	double margin = 0.1;
	char *input_name = NULL;
	char *output_name = "output.txt";
	bool strict = true;

	//odczytanie flag i ich wartosci
	while(argv_index < argc-1)
	{
		//szukanie flag oraz odczytanie wskaznika typu void do wartosci flagi
		void *ptr_flag_value = scan_flags(&flags, argv, argv_index);

		if(ptr_flag_value != NULL)
		{
			//rzutowanie wskaznika typu void na napis
			flag_value = (char*)ptr_flag_value;

			//przypisanie wartosci dla odpowiedniej flagi
			if(flags == 1)
			divide = atoi(flag_value);

			else if(flags == 2)
			margin = atof(flag_value);

			else if(flags == 3)
			input_name = flag_value;
	
			else if(flags == 4)
			output_name = flag_value;

			else if(flags == 5)
			{
				if(*flag_value == '0')
					strict = false;

				else if(*flag_value == '1')
					strict = true;
			}
		}
			//po poprawnym odczytaniu flagi i jej wartosci szukamy nastepnej flagi 
			if(flags != 0)
				argv_index += 2;
	
			else
				++argv_index;
	}

	//sprawdzenie poprawnosci divide oraz margin
	if(divide < 2)
	{
		printf("Nieprawidlowa wartosc flagi divide\n");
		return 1;
	}

	if(margin < 0.0 || margin > 1.0)
	{
		printf("Nieprawidlowa wartosc flagi margin\n");
		return 2;
	}

	if(input_name == NULL)
		return 3;
		
	//otwarcie plik z grafem
	FILE *in = fopen(input_name, "r");

	if(in == NULL)
	{
		printf("Nieprawidlowa nazwa pliku\n");
		return 4;
	}

	int n = 0;
	int nodes = 0;
	double D_norm = 0;
	int connections1 =0;

	//odczytanie maksymalnej ilosc wierzcholkow w jednym wierszu
	fscanf(in, "%d\n", &n);
	
	if(n == 0)
		return 5;

	node_t *t = NULL;

	//alokacja pamieci dla tablicy struktur
	grupa_g g = malloc(divide * sizeof(struct group));
	
	//macierz sasiedztwa
	int **A_matrix = create_A_matrix(in, &nodes, &t, &connections1);
	printf("Macierz Sasiedztwa\n");

	//zamkniecie pliku do odczytu
	fclose(in);

	//ilosc iteracji przy obliczaniu macierzy trojdiagonalnej, oraz jej rozmiar
	int ITERATIONS = 50;

	//rozmiar macierzy trojdiagonalnej musi byc mniejszy rowny rozmiarowi macierzy Laplace'a
	if(nodes <= 50)
		ITERATIONS = nodes;
	
	//kryteria ilosci wierzcholkow w grupach
	int max_nodes = (int)floor((double)nodes/divide*(1+margin)); 	
	int min_nodes = (int)ceil((double)nodes/divide*(1-margin)); 	

	if(max_nodes*divide < nodes)
	{		
		printf("podzial na %d czesci przy marginesie %lf nie jest mozliwy\n",divide,margin);
		printf("prosze zmienic ilosc grup na jakie program ma podzielic graf, badz zwiekszyc margines!\n");
		return 1;
	}

	//alokacja pamieci dla tablic w strukturze
	for(int i = 0; i < divide; i++){
		g[i].gr_nodes = malloc(nodes * sizeof(int)); 
		g[i].no_con = calloc(nodes, sizeof(int));
	}	

	//wektor stopni obliczony na podstawie macierzy sasiedztwa
	int *D_vector = create_D_vector(A_matrix, nodes, &D_norm);
	printf("Wektor Stopni\n");

	int all_edges = 0;
	int nz = 0;
	
	//policzenie ilosci krawedzi w grafie
	for(int i = 0; i < nodes; i++)
	{
		all_edges += D_vector[i];
		nz += (D_vector[i] + 1);
	}

	all_edges /= 2;

	//macierz Laplace'a
	double **L_matrix = create_L_matrix(A_matrix, D_vector, nodes);
	printf("Macierz Laplace'a\n");

	//dodawanie sasiadow
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

	//CSR macierzy Laplace'a
	csr_t compressed_L_matrix = create_compressed_matrix(L_matrix, nz, nodes);
	
	//obliczenie wspolczynnikow alfa i beta
	calculate_coefs(compressed_L_matrix, initial_vec, prev_initial_vec, alfa_coefs, beta_coefs, nodes, 0, ITERATIONS);
	printf("Wspolczynniki alfa i beta\n");

	free(prev_initial_vec);
	free_csr(compressed_L_matrix);

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
	
	printf("Wartosci wlasne\n");

	free_matrix(Q_matrix, ITERATIONS);

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

	//CSR przeksztalconej macierzy Laplace'a
	compressed_L_matrix = create_compressed_matrix(L_matrix, nz, nodes);
	
	//wspolczynniki metody gradientowej
	double learning_rate = 0.001;
	double momentum = 0.8;
	double epsilon_margin = pow(10, -12);

	if(nodes > 5000)
	       epsilon_margin = pow(10, -5);

	//alokacja pamieci dla wektora predkosci
	double *velocity = calloc(nodes, sizeof(double));

	double epsilon = 0.0;
	double prev_epsilon = 0.0;
	double *eigenvector = NULL;

	//wektor wlasny
	do
	{
		eigenvector = calculate_eigenvector(initial_vec, L_matrix, compressed_L_matrix, nodes, learning_rate, momentum, velocity, &epsilon_margin, &epsilon, &prev_epsilon);

	//funkcja wykonuje sie do momentu osiagniecia zadanego marginsesu	
	} while(epsilon > epsilon_margin);

	printf("Wektor wlasny\n");
	
	free(velocity);
	free_csr(compressed_L_matrix);

	//cofniecie zmian w macierzy Laplace'a
	for(int i = 0; i < nodes; i++)
		L_matrix[i][i] += eigenvalue;	

	//przypisanie wartosci wektora wlasnego
	assign_eigen(t, eigenvector, nodes);
	printf("Przydzielenie wartosci wektora wlasnego\n");

	//sortujemy wektor wlasny
	qsort(eigenvector, nodes, sizeof(double), compare);

	//przypisanie grup
	list_gr_con(t,g,nodes,divide,max_nodes,D_vector,eigenvector,L_matrix);	
	printf("Przydzielenie grup\n");

	//obliczenie korzysci rafinacji
	gain_calculate(t, divide, nodes);
	printf("Obliczenie korzysci rafinacji\n");

	//rafinacja
	for(int i = 0; i < 5; i++)	
	{
		find_leaves(t, nodes);
		refine_groups(t, nodes, g, max_nodes, min_nodes);
		gain_calculate(t, divide, nodes);
	}
	printf("Rafinacja grup\n");

	int group_margin = 0;

	//zliczenie ilosci wierzcholkow w kazdej z grup
	int *gr_nodes = count_group_nodes(t, nodes, divide, min_nodes, max_nodes, &group_margin);

	//sprawdzenie czy grupy mieszcza sie w marginesie
	bool in_margin = is_in_margin(g, divide, min_nodes, max_nodes);

	//flaga okreslajaca czy utworzone grup tworza spojne grafy
	bool is_consistent = true;

	//sprawdzenie spojnosci grup
	int *dfs_check = check_gr_con(t,g,divide,nodes);
	for(int i =0; i < divide; i++)
	{
		if(gr_nodes[i] != dfs_check[i])
		{
			is_consistent = false;
			break;
		}
	}

	for(int i = 0; i < nodes; i++)
	{
		if(t[i]->group == -1)
		{
			is_consistent = false;
			break;
		}
	}

	//flaga okreslajaca czy wszystkie grupy maja wiecej niz 1 wierzcholek
	bool is_graph = true;

	//sprawdzenie czy wszystkie grupy tworza grafy
	for(int i = 0; i < divide; i++)
	{
		if(gr_nodes[i] <= 1)
		{
			is_graph = false;
			break;
		}
	}	

	free(dfs_check);
	free(gr_nodes);

	//przy spelnieniu wszystkich wymagow nastepuje zapisanie podzialu do pliku oraz wypisanie rezultatow podzialu
	if(((in_margin || !strict)) && is_consistent && is_graph)
		print_results(t, nodes, divide, A_matrix, n, all_edges, output_name, group_margin, strict);	
	
	else
		printf("\nPodzial grafu zakonczyl sie niepowodzeniem. Zwieksz margines bledu lub zmniejsz ilosc grup\n");

	//zwolnienie pamieci
	free_int_matrix(A_matrix, nodes);
	free_matrix(L_matrix, nodes);
	free(eigenvector);
	free(D_vector);
	free_group(g, divide);
	free_node(t, nodes);
	free(g);

	//zwracamy kod niepowodzenia podzialu grafu
	if((!in_margin && strict) || !is_graph || !is_consistent)
		return 6;
	
	//zwracamy kod powodzenia podzialu grafu
	return 0;
}

