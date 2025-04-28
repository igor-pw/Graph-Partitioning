//#include "headers/matrix.h"
#include "headers/vector.h"

static int depth = 0;

int *create_D_vector(int **matrix, int n, double *D_norm)
{	
	//alokujemy pamiec na n wskaznikow do tablic
        int *D_vector = malloc(sizeof(int) * n);	

	int sum = 0;

	for(int i = 0; i < n; i++)
	{
		sum = 0;

		//dodajemy wszystkie elemnty wiersza tablicy matrix
		for(int j = 0; j < n; j++)
			sum+=matrix[i][j];	
	
		//wstawiamy do wektora wartosc sumy wiersza		
		D_vector[i] = sum;

		//dodajemy kwadrat wartosci sumy wiersza 
		*D_norm += pow(sum, 2); 
	}

	//obliczamy dlugosc wektora
	*D_norm = sqrt(*D_norm);

	return D_vector; 
}

double vec_norm(double *vec, int n)
{
        //norma L2 wektora (dlugosc)

        double sum = 0;

	//suma kwadratow wszystkich elementow wektora
        for(int i = 0; i < n; i++)
                sum += pow(vec[i], 2);

	//zwracamy pierwiastek z sumy
        return sqrt(sum);
}

void print_vec(double *vec, int n)
{
        printf("\n[ ");

        for(int i = 0; i < n; i++)
                printf("%.15f ", vec[i]);

        printf("]\n");
}

double *multiply_mtx_by_vec(double **matrix, double *vec, int n)
{
        //wypelnienie wektora wynikowego zerami
        double *result_vec = calloc(n, sizeof(double));

        for(int i = 0; i < n; i++)
        {
		//dodajemy do i-tego elementu wektora wynikowego wynik mnozenia i,j elemntu macierzy przez j-ty element wektora
                for(int j = 0; j < n; j++)
                        result_vec[i] += matrix[i][j]*vec[j];
	}

        return result_vec;
}

double *multiply_compressed_mtx_by_vec(csr_t csr, double *vec, int n)
{
	double *result_vec = calloc(n, sizeof(double));
		
	for(int i = 0; i < n; i++)
	{
		for(int j = csr->row_ptr[i]; j < csr->row_ptr[i+1]; j++)
		{
			result_vec[i] += csr->values[j]*vec[csr->col_index[j]]; 	
		}
	}

	return result_vec;
}

double multiply_vec_by_vec(double *vec, double *vecT, int n)
{
        //mnozy wektor przez wektor
        double result = 0;

	//sumujemy otrzymane wyniki mnozenia przez siebie i-tych elementow wektora
        for(int i = 0; i < n; i++)
                result += vecT[i]*vec[i];

        return result;
}

double *create_initial_vec(int *D_vector, double D_norm, int n)
{
        //tworzy wektor poczatkowy
	
	//alokacja pamieci
        double *initial_vec = malloc(sizeof(double) * n);

	//initial_vec to znormalizowany wektor stopni (norma L2)
        for(int i = 0; i < n; i++)
                initial_vec[i] = (double)(D_vector[i])/D_norm;

        return initial_vec;
}

void subtract_vec(double *vec, double *coef_vec, double coef, int n)
{
        //odejmuje wektor od wektora pomnozonego przez wspolczynnik

        for(int i = 0; i < n; i++)
                vec[i] -= coef*coef_vec[i];
}

void divide_vec(double *vec, double coef, int n)
{
        //dzieli vec przez wspolczynnik
	
        for(int i = 0; i < n; i++)
                vec[i] /= coef;
}

void copy_vec(double *src_vec, double *dest_vec, int n)
{
        for(int i = 0; i < n; i++)
                dest_vec[i] = src_vec[i];
}

double find_smallest_eigenvalue(double *vec, int n)
{
	//wartosci wlasne sa uporzadkowane od najwiekszej wartosci do najmniejszej wzgledem wartosci bezwzglednej
	//szukamy od konca najmniejszej wartosci dodatniej, ktora bedzie szukana wartoscia wlasna macierzy
	
	double eigenvalue = -1;	

	while(eigenvalue < 0)
	{
		eigenvalue = vec[n-1];
	       	n--;	
	}

	return eigenvalue;
}

double *calculate_eigenvector(double *vec, double **gradient_matrix, csr_t matrix, int n, double learning_rate, double momentum, double *velocity, double *epsilon_margin, double *epsilon, double *prev_epsilon)
{
	//
	double *r_vec = multiply_compressed_mtx_by_vec(matrix, vec, n);
	double *gradient = multiply_compressed_mtx_by_vec(matrix, r_vec, n);

	for(int i = 0; i < n; i++)
		gradient[i] *= 2;

	copy_vec(vec, r_vec, n);

	double vec_norm = 0;
	for(int i = 0; i < n; i++)
	{
		velocity[i] = momentum*velocity[i] + (1 - momentum)*gradient[i];
		vec[i] -= learning_rate*velocity[i];
		vec_norm += vec[i]*vec[i];
	}

	vec_norm = sqrt(vec_norm);
	divide_vec(vec, vec_norm, n);

	*epsilon = 0;

	for(int i = 0; i < n; i++)
		*epsilon += (pow(vec[i]-r_vec[i], 2));

	*epsilon = sqrt(*epsilon);
	
	free(r_vec);
	free(gradient);

	depth++;
	
	if(*epsilon < pow(10, -3) && *epsilon >= *prev_epsilon)
	{
		printf("KONIEC\n");
		*epsilon_margin = pow(10, -3);
		return vec;
	}

	*prev_epsilon = *epsilon;

	if(*epsilon > *epsilon_margin && depth < 10000)
	{	
		vec = calculate_eigenvector(vec, gradient_matrix, matrix, n, learning_rate, momentum, velocity, epsilon_margin, epsilon, prev_epsilon);
	}

	if(depth >= 10000)
	{
		depth = 0;	
		return NULL;
	}
	else
		return vec;
}

void eigen_quantile(double *eigenvector, int n, int k, node_t *t, grupa_g g, double **L_matrix){ //158
	double div = (double)k/n;
	int tmp =0; // potrzebne do obliczenia ostatniej grupy;

	//przydzielamy kazdemu wierzcholkowi gr -1 co jest rownoznaczne z tym, ze wierzcholek nie zostal przydzielony do zadnej grupy	
	for(int i = 0; i< k; i++)
		t[i]->group = -1;
	
	//przechodzimy po kazdej grupie
	for(int i =1; i<n; i++){
		//wyznaczamy zakres na jaki patrzymy w wektorze wartosci wlasnych
		int ind_k = (int)round(i*div);
		int ind_p = (int)round((i-1)*div)+1;
		tmp = ind_k;
		int size = ind_k-ind_p+1;
		double *eigen_tmp = malloc(size * sizeof(double));
		int count = 0;
		//zapisujemy wartosci wlasne z badanego zakresu
		for(int j = ind_p; j <= ind_k; j++){
			eigen_tmp[count] = eigenvector[j];
			count++;
		}
		
		int max_len = -1;
		int node = -1;
		
		int *node_gr = malloc(size * sizeof(int));
		//szukamy wierzcholkow ktore maja dana wartosc wlasna
		for(int j = 0; j < size; j++){
			for(int l =0; l < k; l++){
				if(eigen_tmp[j] == t[l]->eigenvalue){
					node_gr[j] = l;
					break;
				}

			}
		}
		//szukamy wierzcholka z najwieksza liczba polanczen aby wybrac go jako korzen
		for(int j = 0; j< size; j++){
			int nr = node_gr[j];
			if(L_matrix[nr][nr] > max_len){
				max_len = L_matrix[nr][nr];
				node = nr;
			}
		}
		//przydzielamy korzeniowi grupe i wpisujemy go do tablicy z wierzcholkami grupy
		g[i-1].gr_nodes[0] = node;
		g[i-1].gr_size =1;
		t[node]->group = i-1;
		
		free(eigen_tmp);
		free(node_gr);
		}
	//robimy to samo tylko ze dla ostatniej grupy, uwzgledniamy tutaj niedokladnosc wynikajaca z reszty przy dzieleniu na grupy 
	int size = k - tmp;
	double *eigen_tmp = malloc(size * sizeof(double));
	int count = 0;
	for(int j = tmp; j < k; j++){
		eigen_tmp[count] = eigenvector[j];
		count++;
		}
	int max_len = -1;
	int node = -1;

	int *node_gr = malloc(size * sizeof(int));
	for(int j = 0; j < size; j++){
		for(int l =0; l < k; l++){
			if(eigen_tmp[j] == t[l]->eigenvalue){
				node_gr[j] = l;
				break;
			}
		}
	}

	for(int j = 0; j< size; j++){
		int nr = node_gr[j];
		if(L_matrix[nr][nr] > max_len){
			max_len = L_matrix[nr][nr];
			node = nr;
		}
	}

	g[n-1].gr_nodes[0] = node;
	g[n-1].gr_size =1;
	t[node]->group = n-1;

	free(eigen_tmp);
	free(node_gr);
}

int compare(const void *a, const void *b)
{
	//funkcja porownawcza do qsort
	double *n = (double *)a;
	double *m = (double *)b;

	if(*n > *m)
		return 1;
	else if(*n < *m)
		return -1;
	
	return 0;
}
