#include "headers/matrix.h"

void print_results(node_t t, int nodes, int ngroups, int **A_matrix, int max_nodes, int low_nodes, int n, int all_edges){
	
	int lu = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(t[lu].y == i && t[lu].x ==j){
				printf("1 ");
				lu++;
			}
			else
				printf("0 ");
		}
		printf("\n");
	}

	int edges = 0;

		printf("\n");
	for(int k = 0; k <ngroups; k++){
		printf("grupa %d\n", k);
		for(int i =0; i<nodes; i++){
			if( t[i].group == k){
					for(int j = i; j < nodes; j++){
						if(t[j].group == k && A_matrix[i][j] == 1)
						{
							edges++;
							printf("%d-%d\n",i,j);
						}
					}
			}
		}
		printf("\n");

	}


	printf("ilosc wszystkich wierzcholkow: %d\n", nodes);
	for(int i = 0; i < ngroups; i ++){
		int countgr = 0;
		for(int j = 0; j < nodes; j ++){
			if(t[j].group == i){
				countgr++;
			}
		}
		printf("ilosc wierzcholkow w gr %d: %d", i , countgr);
		if(countgr <low_nodes || countgr > max_nodes)
			printf(" <---- grupa niezgonda z marginesem!!!!\n");
		else
			printf("\n");
	}
	int wolne_wieszcholki = 0;
	for(int i =0; i< nodes; i++){
		if(t[i].group == -1)
			wolne_wieszcholki++;
	}
	printf("ilosc wolnych wierzcholkow: %d\n",wolne_wieszcholki);

	printf("poczatkowa ilosc krawedzi: %d\n", all_edges);
	printf("usuniete krawedzie: %d\n", all_edges-edges);
	printf("procent usunietych krawedzi: %g\n", (double)(all_edges-edges)/(double)(all_edges));
}
