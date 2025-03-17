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
	for(int i = 0; i<counter;i++)
		printf("%d ,",index[i]);
	printf("\n");

	c = ';';
	
	while(!isspace(c))
	{
		if(fscanf(in, "%d%c", &index2[counter2], &c) == 2);
			counter2++;
	}
	
	for(int i = 0; i<counter2;i++)
		printf("%d ,",index2[i]);
	printf("\n");
	
//	index2[counter2] = counter2;

        double **matrix = calloc(size, sizeof(double));

        for(int i = 0; i < size; i++)
                matrix[i] = calloc(size, sizeof(double));

	for(int i = 0; i < (counter2 -1) ; i++)
	{
		
		int y = index[index2[i]]; // w index2[] kolejne liczby to wspolzende wieszchlka ktory jest w index[] 
		for(int j = (index2[i] + 1); j < index2[i+1]; j++) // omijamy punkt i bo tam jest wieszcholek z kturego idziemy
								   // a i + 1 bo to wskazuje przedzial ile punktuw jest polonczonych z y
		{
			// napisanie na odwrÃddt Åe np 1 ma polonczenie z 2 jest git
			// ale ze 2 ma poloddnczenie z 1 jest blendne
			//int node_nr = iddndex2[i];
			//int ab = j+1;dd
			int x = index[j]; //przechodzimy po wiezcholkach do kturych punkt y ma przejscie
			matrix[y][x] = 1;
			matrix[x][y] = 1; // np. skoro 0 idzie do 1 to tutaj zaznaczmy ze 1 idzie do 0


			//matrix[index[node_nr]][index[ab]] = 1;
			//matrix[index[ab]][node_nr] = 1;
		}	
	}
	
	*A_size = size;
	return matrix;
}

void print_matrix(double **matrix, int n)
{
	for(int i = 0; i < n; i++)
	{
		printf("| ");

		for(int j = 0; j < n; j++)
			printf("%g ", matrix[i][j]);

		printf("|\n");
	}

	printf("\nAdjancency Matrix %dx%d\n\n", n, n);

}
