#include "headers/matrix.h"
#include "headers/vector.h"
#include "headers/graph.h"
#include <ctype.h>
#define am 512000

int **create_A_matrix(FILE *in, int *nodes, node_t **t, int *connections1)
{
	int size = 0;
	char c = ';';

	//bufor
	int index[am];

	//odczytanie 1 linii pliku i zapisanie do bufora
	while(!isspace(c))
	{
		if(fscanf(in, "%d%c", &index[size], &c) == 2)
			size++;
	}

	int start_index;
	int end_index;
	
	int cl = 0;
	int c2 = 0;

	//alokacja pamieci dla tablicy wskaznikow do struktur
	*t = malloc(size * sizeof(node_t));

	fscanf(in, "%d%c", &start_index, &c); 

	//odczytywanie koncowego indexu i inicjacja struktur wierzcholkow
	do
	{
		if(fscanf(in, "%d%c", &end_index, &c) == 2)
		{
			//obliczenie ilosci wierzcholkow w danym wierszu
			int line = end_index - start_index;
			if(line != 0)
			{
				for(int j = start_index; j < end_index; j++)
				{
					int x = index[j];

					//alokacja pamieci dla struktury wierzcholka
					(*t)[c2] = malloc(sizeof(struct node));
					(*t)[c2]->x = x;
					(*t)[c2]->y = cl;
					(*t)[c2]->eigenvalue = 0.0;
					(*t)[c2]->is_leaf = false;
					c2++;
				}

			}

			cl++;

			//zamiana indeksu koncowego na poczatkowy
			start_index = end_index;
		}

	//odczytujemy do momentu napotkania znaku '\n'
	} while(!isspace(c));
	
	//wyzerowanie bufora
	for(int i = 0; i < size; i++)
		index[i] = 0;

	int counter = 0;	
	//odczytanie 4 linii pliku i zapisanie do bufora
	
	c = ';';
	
	while(!isspace(c))
	{
		if(fscanf(in, "%d%c", &index[counter], &c) == 2)
			counter++;
	} 

	//alokujemy pamiec na size wskaznikow do tablic
        int **matrix = malloc(sizeof(int*) * size);
	//alokujemy pamiec na size elementow i wypelniamy zerami
        for(int i = 0; i < size; i++)
                matrix[i] = calloc(size, sizeof(int));
	
	int countconnect = 0;
	fscanf(in, "%d%c", &start_index, &c);

	//odczytywanie koncowego indexu
	do
	{
		if(fscanf(in, "%d%c", &end_index, &c) == 2)
		{
			//w index2[] kolejne liczby to wspolrzedne wierzcholka ktory jest w index[]	
			int y = index[start_index]; 
			
			//zaczynamy od i + 1, ktory wskazuje przedzial ile punktuw jest polaczonych z y
			for(int i = (start_index + 1); i < end_index; i++) 
			{	
				countconnect++;
				//przechodzimy po wierzcholkach do ktorych punkt y ma przejscie
				int x = index[i]; 
				
				//zapisujemy polaczenie miedzy wierzcholkami do macierzy sasiedztwa
				matrix[y][x] = 1;
				matrix[x][y] = 1; 
			}
			//zamiana indeksu koncowego na poczatkowy
			start_index = end_index;	
		}

		else
			break;
	} while(!isspace(c));	

	//odczytanie ostatniego segmentu polaczen
	int y = index[start_index];

	for(int i = (start_index + 1); i < counter; i++)
	{
		countconnect++;
                int x = index[i]; 
                matrix[y][x] = 1;
                matrix[x][y] = 1; 
	}
	//zapisanie wartosci 
	*connections1 = countconnect;
	*nodes = size;
	return matrix;
}

double **create_L_matrix(int **A_matrix, int *D_vector, int n)
{
	//alokacja pamieci na wskazniki do wierszy
	double **L_matrix = malloc(sizeof(double*) * n);

	//wyzerowanie wszystkich elementow wierszy
	for(int i = 0; i < n; i++)
		L_matrix[i] = calloc(n, sizeof(double));


	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{	
			//w miejsce 1 w macierzy sasiedztwa wstawiamy do macierzy Laplace'a -1	
			if(A_matrix[i][j] == 1)
				L_matrix[i][j] = -1;
		}
		
		//wstawiamy na diagonale wartosci z wektora stopni
		L_matrix[i][i] = D_vector[i];
	}

	return L_matrix;
}

csr_t create_compressed_matrix(double **matrix, int nz, int n)
{
	//nz - ilosc niezerowych elementow macierzy
	//n - ilosc wierszy

	//alokacja pamieci dla struktury
	csr_t csr = malloc(sizeof(*csr));	

	//alokacja pamieci tablic
	csr->values = malloc(sizeof(double) * nz);
	csr->col_index = malloc(sizeof(int) * nz);
	csr->row_ptr = malloc(sizeof(int) * (n+1));

	csr->row_ptr[0] = 0;

	int counter = 0;
	
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			if(matrix[i][j] != 0)
			{
				//zapisanie niezerowego elementu do tablicy
				csr->values[counter] = matrix[i][j];

				//zapisanie jego numeru kolumny
				csr->col_index[counter] = j;

				counter++;
			}
		}
		
		//zapisanie indeksu ostatniego elementu w danym wierszu
		csr->row_ptr[i + 1] = counter;
	}

	return csr;
}

double **create_T_matrix(double *a, double *b, int k)
{ 
	//tworzenie macierzy trojdiagonalnej, k to jest rozmiar wektora a 
	
	//alokacja pamieci
	double **T_matrix = malloc(sizeof(double*) * k);

	for(int i = 0; i < k; i++)
                T_matrix[i] = calloc(k, sizeof(double)); 

	//z wektora b zuzyje sie k-1 elementow
	int t = k-1; 
	
	//wypelnienie macierzy
	for(int i = 0; i < t; i++)
	{
		//wartosci a umieszczamy na diagonali macierzy
		T_matrix[i][i] = a[i];

		//wartosci b umieszaczamy nad oraz pod diagonala macierzy
		T_matrix[i][i+1] = b[i];
		T_matrix[i+1][i] = b[i];
	}

	//dodanie a na k x k miejsce
	T_matrix[t][t] = a[t]; 
	
	return T_matrix;
}

void print_matrix(double **matrix, int n)
{
	//wypisuje macierz
	for(int i = 0; i < n; i++)
	{
		//printf("| ");

		for(int j = 0; j < n; j++)
			printf("%.3f ", matrix[i][j]);

		printf("\n");
	}

	printf("\nMatrix %dx%d\n\n", n, n);
}

void calculate_coefs(csr_t L_matrix, double *initial_vec, double *prev_initial_vec, double *alfa_coefs, double *beta_coefs, int n, int i, int k)
{
	//wektor resztowy
	double *residual_vec = multiply_compressed_mtx_by_vec(L_matrix, initial_vec, n);

	//dla i > 0 odejmujemy poprzedni wektor poczatkowy
	if(i > 0)
		subtract_vec(residual_vec, prev_initial_vec, beta_coefs[i-1], n);

	//wspolczynnik i,i macierzy tridiagonalnej
	alfa_coefs[i] = multiply_vec_by_vec(residual_vec, initial_vec, n);

	//aktualizacja wektora resztowego
	subtract_vec(residual_vec, initial_vec, alfa_coefs[i], n);
	
	//wpolczynnik i-1,i oraz i,i-1 macierzy tridiagonalnej
	beta_coefs[i] = vec_norm(residual_vec, n);

	//kopiujemy zawartosc initial_vec do prev_initial_vec
	copy_vec(initial_vec, prev_initial_vec, n);

	//tworzymy nowy wektor poczatkowy
	divide_vec(initial_vec, beta_coefs[i], n);
	
	//zwiekszamy licznik
	i++;
	
	//wywolujemy rekurencyjnie funkcje k razy
	if(i < k)
		calculate_coefs(L_matrix, initial_vec, prev_initial_vec, alfa_coefs, beta_coefs, n, i, k);

	//zwalniamy pamiec
	free(residual_vec);
}

double **create_G_matrix(double **T_matrix, int n, int x)
{
	//obliczamy promien
	double r = sqrt(pow(T_matrix[x][x], 2) + pow(T_matrix[x+1][x], 2));

	//obliczamy sinus i cosinus
	double s = T_matrix[x+1][x]/r;
	double c = T_matrix[x][x]/r;

	//tworzymy transponowana macierz rotacji Givensa
	
	//alokacja pamieci
	double **G_matrix = malloc(sizeof(double*) * n);
	
	for(int i = 0; i < n; i++)
			G_matrix[i] = calloc(n, sizeof(double));

	//wypelniamy elementy diagonalne jedynkami
	for(int i = 0; i < n; i++)
		G_matrix[i][i] = 1.0;

	//wstawiam obliczone wspolczynniki do macierzy
	G_matrix[x][x] = c;
	G_matrix[x+1][x+1] = c;
	G_matrix[x][x+1] = s;
	G_matrix[x+1][x] = -s;	

	return G_matrix;
}

void transpose_matrix(double **matrix, int n)
{
	//tranospozycja macierzy 
	for(int i = 0; i < n; i++) 
    	{
		//przechodzimy wszystkie elementy poddiagonalne i zamieniamy miejscami ij, ji
        	for(int j = i + 1; j < n; j++)
        	{
            		double temp = matrix[i][j];
            		matrix[i][j] = matrix[j][i];
            		matrix[j][i] = temp;
        	}
    	}
}


void copy_matrix(double **src_matrix, double **dest_matrix, int n)
{
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
			dest_matrix[i][j] = src_matrix[i][j];
	}
}

double **multiply_mtx_by_mtx(double **left_matrix, double **right_matrix, int n)
{
	//mnozenie macierzy przez macierz

	//alokacja pamieci
	double **result_matrix = malloc(sizeof(double*) * n);

	//wypelniamy macierz wynikowa zerami
	for(int i = 0; i < n; i++)
		result_matrix[i] = calloc(n, sizeof(double));

	//dodajemy wyniki mnozenia dla elementu i,k lewej macierzy z elementem k,j prawej macierzy do i,j elementu macierzy wynikowej
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{	
			for(int k = 0; k < n; k++)
					result_matrix[i][j] += left_matrix[i][k]*right_matrix[k][j];
		}
	}

	return result_matrix;
}

void calculate_eigenvalue(double **T_matrix, double **Q_matrix, int n, int i)
{
	//tworzy transponowana macierz rotacji Givensa
	double **G_matrix = create_G_matrix(T_matrix, n, i);

	//tworzy poprzez rekurencje macierz trojkatna gorna
	double **R_matrix = multiply_mtx_by_mtx(G_matrix, T_matrix, n); 

	copy_matrix(R_matrix, T_matrix, n);

	//transponujemy macierz aby otrzymac zwykla macierz rotacji Givensa
	transpose_matrix(G_matrix, n);

	free_matrix(R_matrix, n);

	//przy pierwszym przejsciu funkcje kopiujemy macierz rotacji Givensa do macierzy Q
	if(i == 0)
		copy_matrix(G_matrix, Q_matrix, n);
	
	//dla kolejnych przejsc mnozymy kazda otrzymana macierz rotacji Givensa przez wczesniejsza, aby na koniec otrzymac iloraz wszystkich macierzy rotacji Givensa
	else
	{
		double ** new_Q_matrix = multiply_mtx_by_mtx(Q_matrix, G_matrix, n);
		copy_matrix(new_Q_matrix, Q_matrix, n);
		free_matrix(new_Q_matrix, n);
	}

	free_matrix(G_matrix, n);
	i++;

	//wywolujemy rekurencyjnie funkcje dla zaaktualizowanych macierzy Q
	if(i < n-1)
		calculate_eigenvalue(T_matrix, Q_matrix, n, i);

	else if(i == n-1)
	{
		//tworzymy nowa macierz tridiagonalna T' i kopiujemy ja do macierzy T
		double **new_T_matrix = multiply_mtx_by_mtx(T_matrix, Q_matrix, n);
		copy_matrix(new_T_matrix, T_matrix, n);
		free_matrix(new_T_matrix, n);
	}	
}

void assign_eigen(node_t *t, double *eigenvector, int n){
	for(int i =0; i <n; i++)
			t[i]->eigenvalue=eigenvector[i];
}

//obliczanie gaina
void gain_calculate(node_t *t, int ngroups, int nodes)
{
	int dif[ngroups];
	
	for(int i = 0; i < nodes; i++)
	{
		//zerujemy tablice z ilosca polanczen
		int cur_node_gr = t[i]->group;
		for(int j = 0; j < ngroups; j++)
			dif[j] = 0;
		//przechodzimy po sasiadujacych wierzcholkach i dodajemy ilosc wierzcholkow do grup
		for(int j =0; j <t[i]->con_count; j++)
			dif[t[t[i]->connected[j]]->group]++;
		
		int min = INT_MAX;
		int gr = -1;
		//przechodzimy po ilosciach polaczen do innych grup i szukamy najmniejszej roznicy
		for(int j = 0; j < ngroups; j++)
		{
			if( dif[j] != 0 )
			{
				int tmp = dif[cur_node_gr] - dif[j];
				
				if(tmp < min)
				{
					min = tmp;
					gr = j;
				}
			}
		}
		
		if(gr == cur_node_gr){ // jesli mamy polanczenie tylko z gupa do ktorej nalezymy to gain = ilosci przejsc do tej grupy 
			t[i]->gr_gain = gr;
			t[i]->gain = dif[gr];
		}
		else{
			t[i]->gr_gain = gr;
			t[i]->gain = min;
		}
		
	}
}

void print_gain(node_t *t, int nodes)
{
	int counter = 0;
	for(int i = 0; i < nodes; i++)
	{
		int x = 0;
		for(int j = 0; j < t[i]->con_count; j++)
		{
			if(t[i]->group == t[t[i]->connected[j]]->group)
			       x++;	
		}

		if(t[i]->gain < 0 && x <= 1)
		{
			printf("wierzcholek %d z grupy %d krawedzi %d -> gain group %d gain %d\n",i,t[i]->group,x,t[i]->gr_gain,t[i]->gain);
			counter++;
		}
	}

	printf("wierzcholki z ujemnym i dodatnim gainem: %d\n", counter);
}

void free_matrix(double **matrix, int n)
{
	//zwolnienie pamieci zaalokowanej dla wierszy macierzy
	for(int i = 0; i < n; i++)
		free(matrix[i]);

	//zwolnienie pamieci zaalokowanej dla wskaznikow do wierszy macierzy
	free(matrix);
}

void free_int_matrix(int **matrix, int n)
{
	for(int i = 0; i < n; i++)
		free(matrix[i]);

	free(matrix);
}

void free_csr(csr_t matrix)
{
	//zwolnienie pamieci zaalokowanej dla tablic wewnatrz struktury
	free(matrix->values);
	free(matrix->col_index);
	free(matrix->row_ptr);

	//zwolnienie pamieci zaalokowanej dla struktury
	free(matrix);	
}
