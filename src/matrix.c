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
		if(fscanf(in, "%d%c", &index2[counter2], &c) == 2);
			counter2++;
	}
	
	index2[counter2] = counter2;

        double **matrix = calloc(size, sizeof(double));

        for(int i = 0; i < size; i++)
                matrix[i] = calloc(size, sizeof(double));

	for(int i = 0; i <= counter2; i++)
	{
		for(int j = index2[i]; j < index2[i+1]; j++)
		{
			int node_nr = index2[i];

			matrix[index[node_nr]][index[j+1]] = 1;
			matrix[index[j+1]][node_nr] = 1;
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
