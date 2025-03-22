#include "headers/matrix.h"
#include <ctype.h>


double **create_A_matrix(FILE *in, int *nodes)
{
	//poprawic wyglada kodu, dodac funkcje opowiedzialna za odczytywanie pliku

	int size = 0;
	int nr;
	char c = ';';

	while(!isspace(c))
	{
		if(fscanf(in, "%d%c", &nr, &c) == 2)
			size++;
	}

	c = ';';
	int temp;

	while(!isspace(c))
	{		
		if(fscanf(in, "%d%c", &temp, &c) == 2)
			continue;
	}

	printf("nodes: %d\n", size);
	
	int index[16386]; 

	//zredukowac do jednej tablicy
	int index2[16386];
	
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
	

	printf("Wczytywanie graf\n");

	//alokujemy pamiec na size wskaznikow do tablic
        double **matrix = malloc(sizeof(double*) * size);

	//alokujemy pamiec na size elementow i wypelniamy zerami
        for(int i = 0; i < size; i++)
                matrix[i] = calloc(size, sizeof(double));

	for(int i = 0; i < (counter2-1); i++)
	{	
		int y = index[index2[i]]; // w index2[] kolejne liczby to wspolzende wieszchlka ktory jest w index[] 
		for(int j = (index2[i] + 1); j < index2[i+1]; j++) // omijamy punkt i bo tam jest wieszcholek z kturego idziemy
								   // a i + 1 bo to wskazuje przedzial ile punktuw jest polonczonych z y
		{
			int x = index[j]; //przechodzimy po wiezcholkach do ktorych punkt y ma przejscie
			matrix[y][x] = 1;
			matrix[x][y] = 1; // np. skoro 0 idzie do 1 to tutaj zaznaczmy ze 1 idzie do 0
		}	
	}
	
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

double vec_norm(double *vec, int n)
{ 
	//normalizacja wektora (dlugosc) 
	//poprawilem funkcje bo powinna zwraca wspolczynnik a nie wektor
	
	double sum = 0;
	
	for(int i = 0; i < n; i++)
		sum += pow(vec[i], 2);

	return sqrt(sum);
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
double *vec_sub(double *a, double *b, int k)
{ 
	// odejmowanie wektorow 
	double *sub = calloc(k, sizeof(double));	
	
	for(int i =0; i<k; i++)
		sub[i] = a[i] - b[i];
	
	return sub;
}

void print_matrix(double **matrix, int n)
{
	//wypisuje macierz
	for(int i = 0; i < n; i++)
	{
		printf("| ");

		for(int j = 0; j < n; j++)
			printf("%g ", matrix[i][j]);

		printf("|\n");
	}

	printf("\nMatrix %dx%d\n\n", n, n);
}

void print_vec(double *vec, int n)
{
	printf("\n[ ");

	for(int i = 0; i < n; i++)
		printf("%g ", vec[i]);

	printf("]\n");
}

double *multiply_mtx_by_vec(double **matrix, double *vec, int n)
{
	//mnozy macierz przez wektor
	double *result_vec = calloc(n, sizeof(double));

	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
			result_vec[i] += matrix[i][j]*vec[j]; 
	}

	return result_vec;
}

double multiply_vec_by_vec(double *vec, double *vecT, int n)
{
	//mnozy wektor przez wektor
	double result = 0;

	for(int i = 0; i < n; i++)
		result += vecT[i]*vec[i];

	return result;
}

double *create_initial_vec(double **D_matrix, int D_norm, int n)
{
	//tworzy wektor poczatkowy
	double *initial_vec = malloc(sizeof(double) * n);

	for(int i = 0; i < n; i++)
		initial_vec[i] = D_matrix[i][i]/sqrt((double)D_norm);
	
	return initial_vec;
}

void subtract_vec(double *vec, double *coef_vec, double coef, int n)
{
	//operacja vec - coef_vec * coef
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
