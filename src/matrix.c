#include "headers/matrix.h"
#include "headers/vector.h"
#include "headers/graph.h"
#include <ctype.h>
#define am 153600

int **create_A_matrix(FILE *in, int *nodes, node_t **t, int *connections1)
{
	//poprawic wyglada kodu, dodac funkcje opowiedzialna za odczytywanie pliku
	int size = 0;
	char c = ';';
	int index[am];
	
	while(!isspace(c))
	{
		if(fscanf(in, "%d%c", &index[size], &c) == 2)
			size++;
	}

	int start_index;
	int end_index;
	c = ';';
	
	int cl = 0;
	int c2 = 0;
	*t = malloc(size * sizeof(node_t));

	fscanf(in, "%d%c", &start_index, &c); 

	while(!isspace(c))
	{
		if(fscanf(in, "%d%c", &end_index, &c) == 2)
		{
			int line = end_index - start_index;
			if(line != 0)
			{
				for(int j = start_index; j < end_index; j++)
				{
					int x = index[j];
					(*t)[c2] = malloc(sizeof(struct node));
					(*t)[c2]->x = x;
					(*t)[c2]->y = cl;
					(*t)[c2]->nr = c2;
					(*t)[c2]->index = c2;
					(*t)[c2]->eigenvalue = 0.0;
					c2++;
				}

			}

			cl++;
			start_index = end_index;
		}
	}

	for(int i = 0; i < size; i++)
		index[i] = 0;

	c = ';';
	int counter = 0;
	
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
	c = ';';
	
	fscanf(in, "%d%c", &start_index, &c);

	while(!isspace(c))
	{
		if(fscanf(in, "%d%c", &end_index, &c) == 2)
		{	
			int y = index[start_index]; // w index2[] kolejne liczby to wspolzende wieszchlka ktory jest w index[]
			for(int i = (start_index + 1); i < end_index; i++) // omijamy punkt i bo tam jest wieszcholek z kturego idziemy a i + 1 bo to wskazuje przedzial ile punktuw jest polonczonych z y
			{	
				countconnect++;
				int x = index[i]; //przechodzimy po wiezcholkach do ktorych punkt y ma przejscie
				matrix[y][x] = 1;
				matrix[x][y] = 1; // np. skoro 0 idzie do 1 to tutaj zaznaczmy ze 1 idzie do 0
			}

			start_index = end_index;	
		}

		else
			break;
	}	
	
	int y = index[start_index];

	for(int i = (start_index + 1); i < counter; i++)
	{
		countconnect++;
                int x = index[i]; //przechodzimy po wiezcholkach do ktorych punkt y ma przejscie
                matrix[y][x] = 1;
                matrix[x][y] = 1; // np. skoro 0 idzie do 1 to tutaj zaznaczmy ze 1 idzie do 0
	}

	*connections1 = countconnect;
	*nodes = size;
	return matrix;
}

double **create_L_matrix(int **A_matrix, int *D_vector, int n)
{
	double **L_matrix = malloc(sizeof(double*) * n);

	for(int i = 0; i < n; i++)
		L_matrix[i] = calloc(n, sizeof(double));

	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{		
			if(A_matrix[i][j] == 1)
				L_matrix[i][j] = -1;
		}

		L_matrix[i][i] = D_vector[i];
	}

	return L_matrix;
}

csr_t create_compresed_matrix(double **matrix, int nz, int n)
{
	csr_t csr = malloc(sizeof(*csr));	
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
				csr->values[counter] = matrix[i][j];
				csr->col_index[counter] = j;
				counter++;
			}
		}
			
		csr->row_ptr[i + 1] = counter;
	}

	printf("nz: %d\n", counter);

	return csr;
}

double **create_T_matrix(double *a, double *b, int k){ //tworzenie macierzy trojdiagonalnej, k to jest rozmiar wektora a 
	//dodane do ulatwienia testowania funkcji	
	double **tri = malloc(sizeof(double*) * k);

	for(int i = 0; i < k; i++)
                tri[i] = calloc(k, sizeof(double)); 
	
	int t = k-1; //z wektora b zuzyje sie k-1 elementow
	for(int i = 0; i<t; i++){// wypelnienie macierzy
		tri[i][i] = a[i];//wartosci a znajduja sie na diagonali
		tri[i][i+1] = b[i];//wartosci b sa pod i po prawo od wartosci b
		tri[i+1][i] = b[i];
	}
	tri[t][t] = a[t]; //dodanie a na k x k miejsce bo nie moze byc b pod nim i obok
	return tri;

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
	double *residual_vec = multiply_compresed_mtx_by_vec(L_matrix, initial_vec, n);

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
	double **G_matrix = malloc(sizeof(double*) * n);
	
	for(int i = 0; i < n; i++)
			G_matrix[i] = calloc(n, sizeof(double));

	for(int i = 0; i < n; i++)
		G_matrix[i][i] = 1.0L;

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
	double **result_matrix = malloc(sizeof(double*) * n);

	for(int i = 0; i < n; i++)
		result_matrix[i] = calloc(n, sizeof(double));

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
	
	//pozniej mnozymy kazda otrzymana macierz rotacji Givensa przez wczesniejsz, na koniec otrzymamy iloraz wszystkich macierzy rotacji Givensa
	else
	{
		double ** new_Q_matrix = multiply_mtx_by_mtx(Q_matrix, G_matrix, n);
		copy_matrix(new_Q_matrix, Q_matrix, n);
		free_matrix(new_Q_matrix, n);
	}

	free_matrix(G_matrix, n);
	i++;

	if(i < n-1)
		calculate_eigenvalue(T_matrix, Q_matrix, n, i);

	else if(i == n-1)
	{
		//tworzymy nowa macierz tridiagonalna T' i kopiujemy ja do starej macierzy T
		double **new_T_matrix = multiply_mtx_by_mtx(T_matrix, Q_matrix, n);
		copy_matrix(new_T_matrix, T_matrix, n);
		//pozbywamy sie bledow numerycznych
		//force_zeros(T_matrix, n, 0.001);
		free_matrix(new_T_matrix, n);
	}	
}

void assing_eigen(node_t *t, double *eigenvector, int n){
	for(int i =0; i <n; i++)
			t[i]->eigenvalue=eigenvector[i];
}

//	|	x	|	y	|	z	|
/*
void assing_group(node_t t, int n, int ngr, double *centyle){
	for(int i =0; i<n; i++){
			for(int j = 1; j <= ngr + 1; j++){
				if(t[i].eigenvalue >= centyle[j-1] && t[i].eigenvalue < centyle[j])
					t[i].group = j -1;
			}
	}
	
}
*/
/*
void connections(node_t t, int n, int **A_matrix, int *connections2){
	int v[n];
	int count = 0;
	for(int i =0; i < n; i++){
		for(int j =i; j < n; j++){
			if(t[i].group == t[j].group && A_matrix[i][j] == 1){
				v[count] = j;
				count++;
			}
		}
		t[i].connected=malloc(count*sizeof(int));
		for(int j = 0; j<count; j++)
			t[i].connected[j]=v[j];
		t[i].vle=count;
		*connections2 +=count;
		count =0;
	}
}
*/
void gain_calculate(node_t *t, int ngroups, int nodes)
{
	int dif[ngroups];
	
	for(int i = 0; i < nodes; i++)
	{
		int cur_node_gr = t[i]->group;
		for(int j = 0; j < ngroups; j++)
			dif[j] = 0;
		
		for(int j =0; j <t[i]->con_count; j++)
			dif[t[t[i]->connected[j]]->group]++;

		int min = INT_MAX;
		int gr = -1;
		for(int j = 0; j < ngroups; j++)
		{
			if( dif[j] != 0 )
			{
				//printf("cur_node_gr = %d, j = %d, ngroups = %d, i =%d, t[i]->group = %d\n",cur_node_gr,j,ngroups,i,t[i]->group);
				int tmp = dif[cur_node_gr] - dif[j];
				
				if(tmp < min)
				{
					min = tmp;
					gr = j;
				}
			}
		}
		
		if(gr == cur_node_gr){ // jesli mamy polonczenie tylko z gupa do ktorej nalezymy to gain = ilosci przejsc do tej grupy 
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
	for(int i =0; i < nodes; i++)
	{
		int x = 0;
		for(int j = 0; j < t[i]->con_count; j++)
		{
			if(t[i]->group == t[t[i]->connected[j]]->group)
			       x++;	
		}

		if(t[i]->gain <= 0)
		{
			printf("wieszcholek %d z grupy %d krawedzi %d -> gain group %d gain %d\n",i,t[i]->group,x,t[i]->gr_gain,t[i]->gain);
			counter++;
		}
	}

	printf("wierzcholki z ujemnym i dodatnim gainem: %d\n", counter);
}

void free_matrix(double **matrix, int n)
{
	for(int i = 0; i < n; i++)
		free(matrix[i]);

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
	free(matrix->values);
	free(matrix->col_index);
	free(matrix->row_ptr);
	free(matrix);	
}
