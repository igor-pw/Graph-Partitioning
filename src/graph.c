#include "headers/matrix.h"
#include "headers/graph.h"

void print_results(node_t *t,grupa_g g, int nodes, int ngroups, int **A_matrix, int max_nodes, int low_nodes, int n, int all_edges)
{	
	int lu = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(lu < nodes && (t[lu]->y == i && t[lu]->x == j)){
				printf("1 ");
				lu++;
			}
			else
				printf("0 ");

			printf("%d\n", lu);
		}
	}

	printf("wypisywanie macierzy\n");

	int edges = 0;

		printf("\n");
	for(int k = 0; k <ngroups; k++){
		printf("grupa %d\n", k);
		for(int i =0; i<nodes; i++){
			if( t[i]->group == k){
					for(int j = i; j < nodes; j++){
						if(t[j]->group == k && A_matrix[i][j] == 1)
						{
							edges++;
							printf("%d-%d\n",i,j);
						}
					}
			}
		}
		printf("\n");

	}

	int group_margin = 0;
	int *gr_nodes = malloc(ngroups*sizeof(int));
	printf("ilosc wszystkich wierzcholkow: %d\n", nodes);
	for(int i = 0; i < ngroups; i ++){
		int countgr = 0;
		for(int j = 0; j < nodes; j ++){
			if(t[j]->group == i){
				countgr++;
			}
		}
		gr_nodes[i]=countgr;
		if(countgr <low_nodes || countgr > max_nodes)
		{
			group_margin++;
			printf("ilosc wierzcholkow w gr %d: %d", i , countgr);
			printf(" <---- grupa niezgonda z marginesem!!!!\n");
		}
		//else
		//	printf("\n");
	}

	int *dfs_check = check_gr_con(t,g,ngroups,nodes);
	for(int i =0; i<ngroups; i++){
		if(gr_nodes[i] != dfs_check[i]){
			printf("gr.%d nie jest ciagla!!!\n ilosc wieszcholkow w grupie = %d, \t ilosc wieszcholkow od korzenia policzonych z dfs-a = %d\n",i,gr_nodes[i],dfs_check[i]);
		}
	//	printf("ilosc wieszcholkow w gr.%d = %d, a z dfs = %d\ \n",i,gr_nodes[i],dfs_check[i]);
	}

	printf("ilosc grup niezgodnych z marginesem: %d\n", group_margin);

	int wolne_wieszcholki = 0;
	for(int i =0; i< nodes; i++){
		if(t[i]->group == -1)
			wolne_wieszcholki++;
	}
	printf("ilosc wolnych wierzcholkow: %d\n",wolne_wieszcholki);

	printf("poczatkowa ilosc krawedzi: %d\n", all_edges);
	printf("usuniete krawedzie: %d\n", all_edges-edges);
	printf("procent usunietych krawedzi: %g\n", (double)(all_edges-edges)/(double)(all_edges));
	free(gr_nodes);
	free(dfs_check);
}

void find_leaves(node_t *node, int n)
{
	for(int i = 0; i < n; i++)
	{
		int counter = 0;
		
		for(int j = 0; j < node[i]->con_count; j++)
		{
			if(node[node[i]->connected[j]]->group == node[i]->group)
				counter++;
		}

		if(counter == 1)
			node[i]->is_leaf = true;	
	}
}

void free_struct_node(node_t *t, int n)
{
	for(int i = 0; i < n; i++)
		free(t[i]);

	free(t);
}
