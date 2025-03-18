#include "headers/matrix.h"
#include <ctype.h>


double **create_A_matrix(int n, FILE *in, int *A_size)
{
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

	int index[8192]; 
	int index2[8192];
	
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
	}
	
	//index2[counter2] = counter2;

        double **matrix = calloc(size, sizeof(double));

        for(int i = 0; i < size; i++)
                matrix[i] = calloc(size, sizeof(double));

	for(int i = 0; i < (counter2 -1) ; i++)
	{
		
		int y = index[index2[i]]; // w index2[] kolejne liczby to wspolzende wieszchlka ktory jest w index[] 
		for(int j = (index2[i] + 1); j < index2[i+1]; j++) // omijamy punkt i bo tam jest wieszcholek z kturego idziemy
								   // a i + 1 bo to wskazuje przedzial ile punktuw jest polonczonych z y
		{
			int x = index[j]; //przechodzimy po wiezcholkach do kturych punkt y ma przejscie
			matrix[y][x] = 1;
			matrix[x][y] = 1; // np. skoro 0 idzie do 1 to tutaj zaznaczmy ze 1 idzie do 0
		}	
	}
	
	*A_size = size;
	return matrix;
}

double **diagonal_matrix(double **matrix, int A_size, int *D_norm){
	
        double **dig = calloc(A_size, sizeof(double));	
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

double **subtract_matrix(double **matrix1, double **matrix2, int A_size){
	
        double **sub = calloc(A_size, sizeof(double));	
	for(int i = 0; i < A_size; i++)
                sub[i] = calloc(A_size, sizeof(double)); // tworzymy macierz o rozmiarze A_size i wypelniamy ja zerami
	
	for(int i = 0; i < A_size; i++){
		for(int j = 0; j < A_size; j++){
			sub[i][j] = matrix1[i][j] - matrix2[i][j]; // odejmujemy od elemntu macierzy1 elemnt macierzy2
		}
	}
	return sub; // zwracamy roznice macierzy
}
double *vector_norm(double *v, int size){ //normalizacja wektora
	int lenn = 0;
	for(int i=0; i <size; i++)
		lenn += pow(v[i], 2);
	lenn = sqrt(lenn);
	//dodane do ulatwienia testowania funkcji	
	double *norm = calloc(size, sizeof(double));	
	//dodane
	for(int i = 0; i<size;i++)
		norm[i] = v[i]/lenn;
	return norm;
}

double **tri_matrix(double *a, double *b, int k){ //tworzenie macierzy trojdiagonalnej, k to jest rozmiar wektora a 
	//dodane do ulatwienia testowania funkcji	
	double **tri = calloc(k, sizeof(double));	
	for(int i = 0; i < k; i++)
                tri[i] = calloc(k, sizeof(double)); 
	//dodane
	
	int t = k-1; //z wektora b zuzyje sie k-1 elementow
	for(int i = 0; i<t; i++){// wypelnienie macierzy
		tri[i][i] = a[i];//wartosci a znajduja sie na diagonali
		tri[i][i+1] = b[i];//wartosci b sa pod i po prawo od wartosci b
		tri[i+1][i] = b[i];
	}
	tri[t][t] = a[t]; //dodanie a na k x k miejsce bo nie moze byc b pod nim i obok
	return tri;

}
double *vec_sub(double *a, double *b, int k){ // odejmowanie wektorow 
	//dodane do ulatwienia testowania funkcji	
	double *sub = calloc(k, sizeof(double));	
	//dodane
	for(int i =0; i<k; i++){
		sub[i] = a[i] - b[i];
	}
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

double *multiply_mtx_by_vec(double **matrix, int n, double *vec)
{
	//mnozy macierz przez wektor
	double *result_vec = malloc(sizeof(double) * n);

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
