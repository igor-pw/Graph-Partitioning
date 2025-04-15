#include "headers/vector.h"

//nie wiem czemu nie wczytuje z bibliteki
#define M_PI 3.14159265358979323846

double vec_norm(double *vec, int n)
{
        //normalizacja wektora (dlugosc)
        //poprawilem funkcje bo powinna zwraca wspolczynnik a nie wektor

        double sum = 0;

        for(int i = 0; i < n; i++)
                sum += pow(vec[i], 2);

        return sqrt(sum);
}
double *vec_sub(double *a, double *b, int k)
{
        // odejmowanie wektorow
        double *sub = calloc(k, sizeof(double));

        for(int i =0; i<k; i++)
                sub[i] = a[i] - b[i];

        return sub;
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

double find_smallest_eigenvalue(double *vec, int n)
{
	//wartosci wlasne sa uporzadkowane od najwiekszej wartosci do najmniejszej wzgledem wartosci bezwzglednej
	//dlatego szukamy od konca najmniejszej wartosci dodatniej, ktora bedzie szukana wartoscia wlasna macierzy
	
	double eigenvalue = -1;	

	while(eigenvalue < 0)
	{
		eigenvalue = vec[n-1];
	       	n--;	
	}

	return eigenvalue;
}

double *calculate_eigenvector(double *vec, double **gradient_matrix, int n, double learning_rate, double momentum, double *velocity)
{
	//nowa wersja w trakcie
	double *r_vec = multiply_mtx_by_vec(gradient_matrix, vec, n);
	double *gradient = multiply_mtx_by_vec(gradient_matrix, r_vec, n);

	for(int i = 0; i < n; i++)
		gradient[i] *= 2;

	double *new_velocity = malloc(sizeof(double) * n);	

	copy_vec(vec, r_vec, n);

	double vec_norm = 0;
	for(int i = 0; i < n; i++)
	{
		new_velocity[i] = momentum*velocity[i] + (1 - momentum)*gradient[i]; 
		vec[i] -= learning_rate*new_velocity[i];
		vec_norm += pow(vec[i], 2);
	}
		
	//copy_vec(new_velocity, velocity, n);

	vec_norm = sqrt(vec_norm);

	divide_vec(vec, vec_norm, n);

	double epsilon = 0;

	for(int i = 0; i < n; i++)
		epsilon += (pow(vec[i]-r_vec[i], 2));

	//printf("epsilon: %g\n", sqrt(fabs(epsilon)));

	free(r_vec);
	free(gradient);
	free(velocity);
	
	if(sqrt(fabs(epsilon)) > pow(10, -6))	
		vec = calculate_eigenvector(vec, gradient_matrix, n, learning_rate, momentum, new_velocity);
	
	return vec;

}

double calculate_median(double *eigenvector, int groups, int n)
{
        double median;

        if(n % 2 == 1)
                median = eigenvector[n/groups+1];

        else if(n % 2 == 0)
                median = (eigenvector[n/groups] + eigenvector[n/groups+1])/2;

        return median;
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
