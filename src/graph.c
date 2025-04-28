#include "headers/matrix.h"
#include "headers/graph.h"

void print_results(node_t *t, int nodes, int ngroups, int **A_matrix, int n, int all_edges, char *file_name, int group_margin, bool strict)
{	
	FILE *out = fopen(file_name, "w");

	int lu = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(lu < nodes && (t[lu]->y == i && t[lu]->x == j)){
				fprintf(out, "1 ");
				lu++;
			}
			else
				fprintf(out, "0 ");
		}
		fprintf(out, "\n");
	}
	fprintf(out, "\n");
	

	printf("Zapisanie podzielonego grafu do pliku\n");

	int edges = 0;

		printf("\n");
	for(int k = 0; k <ngroups; k++){
		fprintf(out, "grupa %d\n", k);
		for(int i =0; i<nodes; i++){
			if( t[i]->group == k){
					for(int j = i; j < nodes; j++){
						if(t[j]->group == k && A_matrix[i][j] == 1)
						{
							edges++;
							fprintf(out, "%d-%d\n",i,j);
						}
					}
			}
		}
		fprintf(out, "\n");

	}

	fclose(out);

	printf("Rezultaty:\n");

	if(group_margin > 0 && !strict)
		printf("ilosc grup niezgodnych z marginesem: %d\n", group_margin);

	int wolne_wieszcholki = 0;
	for(int i =0; i< nodes; i++){
		if(t[i]->group == -1)
			wolne_wieszcholki++;
	}

	printf("poczatkowa ilosc krawedzi: %d\n", all_edges);
	printf("usuniete krawedzie: %d\n", all_edges-edges);
	printf("procent usunietych krawedzi: %g\n", (double)(all_edges-edges)/(double)(all_edges));
}

int dfs(node_t *t, int node, int gr, int *hbs)
{
	int count = 1;
	hbs[node]=1;
	for(int i =0; i <t[node]->con_count ; i++){
		int nb = t[node]->connected[i];
		if(t[nb]->group == gr && hbs[nb] == 0){
			count += dfs(t,nb,gr,hbs);
		}
	}
	return count;
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

int *count_group_nodes(node_t *node, int nodes, int divide, int min_nodes, int max_nodes, int *group_margin)
{	
	int *gr_nodes = malloc(divide*sizeof(int));
	
	for(int i = 0; i < divide; i++)
	{
		int countgr = 0;
		
		for(int j = 0; j < nodes; j++)
		{
			if(node[j]->group == i)
				countgr++;
		}
		
		gr_nodes[i]=countgr;
		
		if(countgr < min_nodes || countgr > max_nodes)
			(*group_margin)++;
	}
	
	return gr_nodes;
}

void free_node(node_t *t, int n)
{
	for(int i = 0; i < n; i++)
	{
		//zwolnienie zaalokowanej w strukturze tablicy z numerami sasiadow
		free(t[i]->connected);

		//zwolnienie pamieci zaalokowanej dla struktury wierzcholka
		free(t[i]);
	}

	//zwolnienie pamieci zaalokowanej dla wskaznikow do struktur wierzcholkow
	free(t);
}

