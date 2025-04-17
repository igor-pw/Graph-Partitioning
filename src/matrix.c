#include "headers/matrix.h"
#include "headers/graph.h"
#include <ctype.h>
#define am 16386

double **create_A_matrix(FILE *in, int *nodes, node_t *t, int *connections1)
{
	//poprawic wyglada kodu, dodac funkcje opowiedzialna za odczytywanie pliku

	int size = 0;
	char c = ';';
	int position[am];
	int position2[am];
	while(!isspace(c))
	{
		if(fscanf(in, "%d%c", &position[size], &c) == 2)
			size++;
	}

	c = ';';
	int temp = 0;
	*t = (node_t)malloc(size * sizeof(struct node));

	while(!isspace(c))
	{		
		if(fscanf(in, "%d%c", &position2[temp], &c) == 2)
			temp++;
	}
	int cl = 0;
	int c2 = 0;
	for(int i = 1; i<temp;i++){
		int line = position2[i] - position2[i-1];
		if(line != 0){
			for(int j = position2[i-1]; j<position2[i];j++){
			int x = position[j];
			(*t)[c2].x = x;
			(*t)[c2].y = cl;
			(*t)[c2].nr = c2;
			(*t)[c2].index = c2;
			(*t)[c2].eigenvalue = 0.0;
			c2++;
			}
		}
		cl++;
	}

	
	int index[am]; 

	//zredukowac do jednej tablicy
	int index2[am];
	
	c = ';';
	int counter = 0;
	int counter2 = 0;
	
	while(!isspace(c))
	{
		if(fscanf(in, "%d%c", &index[counter], &c) == 2)
			counter++;
	}

	c = ';';
	
	while(!isspace(c))
	{
		if(fscanf(in, "%d%c", &index2[counter2], &c) == 2)
			counter2++;
		else
			break;
	}
	

	printf("Wczytywanie grafu\n");

	//alokujemy pamiec na size wskaznikow do tablic
        double **matrix = malloc(sizeof(double*) * size);

	//alokujemy pamiec na size elementow i wypelniamy zerami
        for(int i = 0; i < size; i++)
                matrix[i] = calloc(size, sizeof(double));
	int countconnect = 0;
	for(int i = 0; i < (counter2-1); i++)
	{	
		int y = index[index2[i]]; // w index2[] kolejne liczby to wspolzende wieszchlka ktory jest w index[] 
		for(int j = (index2[i] + 1); j < index2[i+1]; j++) // omijamy punkt i bo tam jest wieszcholek z kturego idziemy a i + 1 bo to wskazuje przedzial ile punktuw jest polonczonych z y
		{	
			countconnect++;
			int x = index[j]; //przechodzimy po wiezcholkach do ktorych punkt y ma przejscie
			matrix[y][x] = 1;
			matrix[x][y] = 1; // np. skoro 0 idzie do 1 to tutaj zaznaczmy ze 1 idzie do 0
		}	
	}

	*connections1 = countconnect;
	*nodes = size;
	return matrix;
}

double **create_D_matrix(double **matrix, int A_size, int *D_norm)
{	
	//alokujemy pamiec na n wskaznikow do tablic
        double **dig = malloc(sizeof(double*) * A_size);	

        for(int i = 0; i < A_size; i++)
                dig[i] = calloc(A_size, sizeof(double)); // tworzymy macierz o rozmiarze A_size i wypelniamy ja zerami
	int sum = 0;

	for(int i = 0; i < A_size; i++){
		sum = 0;
		for(int j = 0; j < A_size; j++){ 
			sum+=matrix[i][j];	// dodajemy wszystkie elemnty wiersza tablicy matrix
		}
		dig[i][i] = sum;	// ustawiamy na lini diagonalnej wartosc sumy wiersza		
		*D_norm += pow(sum, 2); //potrzebne do obliczenia wektora poczatkowego (normalizacja L2) 
	}
	return dig; // zwracamy tablice
}

double **subtract_matrix(double **matrix1, double **matrix2, int n)
{
	//alokujemy pamiec na n wskaznikow do tablic	
        double **sub = malloc(sizeof(double*) * n);	
	for(int i = 0; i < n; i++)
                sub[i] = calloc(n, sizeof(double)); // tworzymy macierz o rozmiarze n i wypelniamy ja zerami
	
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			sub[i][j] = matrix1[i][j] - matrix2[i][j]; // odejmujemy od elemntu macierzy1 elemnt macierzy2
		}
	}
	return sub; // zwracamy roznice macierzy
}

double **tri_matrix(double *a, double *b, int k){ //tworzenie macierzy trojdiagonalnej, k to jest rozmiar wektora a 
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

void calculate_coefs(double **L_matrix, double *initial_vec, double *prev_initial_vec, double *alfa_coefs, double *beta_coefs, int n, int i, int k)
{
	//wektor resztowy
	double *residual_vec = multiply_mtx_by_vec(L_matrix, initial_vec, n);

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

void force_zeros(double **matrix, int n, double margin)
{
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
			if(i != j && matrix[i][j] <= margin && matrix[i][j] >= -margin)
					matrix[i][j] = 0.f;
	}

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

	i++;

	if(i < n-1)
		calculate_eigenvalue(T_matrix, Q_matrix, n, i);

	else if(i == n-1)
	{
		//tworzymy nowa macierz tridiagonalna T' i kopiujemy ja do starej macierzy T
		double **new_T_matrix = multiply_mtx_by_mtx(T_matrix, Q_matrix, n);
		copy_matrix(new_T_matrix, T_matrix, n);
		//pozbywamy sie bledow numerycznych
		force_zeros(T_matrix, n, 0.01);
		free_matrix(new_T_matrix, n);
	}	
}

double **create_I_matrix(int n, double coef)
{
	//stworzenie macierzy jednostkowej przemnozonej przez wspolczynnik
	double **I_matrix = malloc(sizeof(double*) * n);

	for(int i = 0; i < n; i++)
		I_matrix[i] = calloc(n, sizeof(double));

	for(int i = 0; i < n; i++)
		I_matrix[i][i] = coef;

	return I_matrix;
}

void assing_eigen(node_t t, double *eigenvector, int n){
	for(int i =0; i <n; i++)
			t[i].eigenvalue=eigenvector[i];
}

//	|	x	|	y	|	z	|

void assing_group(node_t t, int n, int ngr, double *centyle){
	for(int i =0; i<n; i++){
			for(int j = 1; j <= ngr + 1; j++){
				if(t[i].eigenvalue >= centyle[j-1] && t[i].eigenvalue < centyle[j])
					t[i].group = j -1;
			}
	}
	
}

void connections(node_t t, int n, double **A_matrix, int *connections2){
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

void gain_calculate(node_t t, double **Macierz_s, int ngroups, int nodes){
	int dif[ngroups];
	for(int i =0; i < nodes; i++){
		int cur_node_gr = t[i].group;
		for(int j = 0; j < ngroups; j++){
			dif[j] = 0;
		}
		for(int j = 0; j < nodes; j++){
			if(Macierz_s[i][j] == 1) dif[t[j].group]++;	
		}
		int min = INFINITY;
		int gr = -1;
		for(int j = 0; j < ngroups; j++){
			if( dif[j] != 0 ){
				int tmp = dif[cur_node_gr] - dif[j];
				if(tmp < min){
					min = tmp;
					gr = j;
				}
			}
		}
		if(gr == cur_node_gr){ // jesli mamy polonczenie tylko z gupa do ktorej nalezymy to gain = ilosci przejsc do tej grupy 
			t[i].gr_gain = gr;
			t[i].gain = dif[gr];
		}
		else{
			t[i].gr_gain = gr;
			t[i].gain = min;
		}
		
	}
}

void print_gain(node_t t, int nodes){
	for(int i =0; i < nodes; i++){
		printf("wieszcholek %d z grupy %d -> gain group %d gain %d\n",i,t[i].group,t[i].gr_gain,t[i].gain);
	}
}

void free_matrix(double **matrix, int n)
{
	for(int i = 0; i < n; i++)
		free(matrix[i]);

	free(matrix);
}
