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

double *calculate_eigenvector(double eigenvalue, int n, double margin)
{
        double *eigenvector = malloc(sizeof(double) * n);
        int N = sqrt(n);
	//G-cut 2000 coppyright
        double p, q;
        bool found = false;

        //prawdopodobnie trzeba ulepszyc proces szukania p i q, albo porownywac wyniki roznych p i q

        for(int i = 1; i < N; i++)
        {
                for(int j = 1; j < N; j++)
                {
                        if(fabs(eigenvalue-4+2*cos((i*M_PI)/(n+1))+2*cos((j*M_PI)/(n+1))) < margin)
                        {
                                p = i;
                                q = j;
                                //printf("p: %g, q: %g\n", p, q);

                                found = true;
                                break;
                        }
                }

                if(found)
                        break;
        }

        if(!found)
        {
                printf("Nie znaleziono odpowiednich wartosci\n");
                return NULL;
        }

        int index;

        for(int i = 1; i <= N+2; i++)
        {
                for(int j = 1; j <= N+2; j++)
                {
                        index = j + (i-1)*N-1;

                        if(index < n)
                                eigenvector[index] = sin((p*M_PI*i)/(N+1))*sin((q*M_PI*j)/(N+1));
                }
        }

        //printf("index: %d\n", index);

        return eigenvector;
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

void eigen_centyl(double *centyle, int n, double *v, int k){ //158
	int div = (int)round(k/n);
	for(int i =0; i<n; i++){
		centyle[i] = v[i*div];
		printf("podzial -> %lf\n",centyle[i]);
	}
	centyle[n]=v[k-1];
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
