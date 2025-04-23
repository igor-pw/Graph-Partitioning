#include "headers/groups.h"
#include <limits.h>

/*
//inicjalizacja grup, -1 oznacza ze wieszcholek nie ma przydzielonej zadnej grupy a na koncu przydziale wieszcholki roota
void ingr(node_t t, int nodes, int ngroups, double *root_val){
	for(int i =0; i <nodes; i++)
		t[i].group = -1;

	}
*/

/*
void first_con(node_t t, double **Macierz_s, int nodes, int ngroups){
	double min_dif = -1.0;
	for(int i =0; i < ngroups; i++){
		for(int j =0; j < nodes; j++){
			double tmp = 		
		}
	}
}
*/


//!!!!!!!!!	TU JEST COS STROGO NIE TAK
//POPRAWIE JUTRO™

int con_am(node_t t, int **A_matrix, int nodes, int node){
	int tmp = 0;
	int cur_gr = t[node].group;
	for(int i = 0; i < nodes; i++){
		if(A_matrix[node][i] != 0 && t[i].group == -1){
			tmp++;
		}
	}
	return tmp;
}

void find_least_con(node_t t, int **A_matrix, int nodes, int node, int *best_node, int * low_con){
	int cur_gr = t[node].group;
	for(int i = 0; i < nodes; i++){
		//printf("gr = %d, Macierz = %d, czy ma gr = %d, badany node = %d\n",cur_gr, Macierz_s[node][i], t[i].group, node);
		if(A_matrix[node][i] != 0 && t[i].group == -1){
			int tmp = con_am(t, A_matrix, nodes, i);
			if(tmp < (*low_con)){
				//printf("tmp = %d, i = %d, node = %d\n",tmp,i,node);
				(*low_con) = tmp;
				(*best_node) = i;
			}
		}
		
	}
}

void find_smallest_con(node_t t, grupa_g g, int **A_matrix, int nodes, int ngroups, int max_gr_size){
	for(int i =0; i<max_gr_size; i++){
		for(int j =0 ; j<ngroups; j++){
			int best_node = INT_MAX;
			int low_con = INT_MAX;
			for(int k = 0; k < g[j].gr_size; k++){
				printf("gr = %d, wiesz = %d\n",j,k);
				find_least_con(t, A_matrix, nodes, g[j].gr_nodes[k], &best_node, &low_con);
			}
			//printf("best node = %d, low con = %d\n",best_node,low_con);
			if(best_node != INT_MAX && low_con != INT_MAX){
			g[j].gr_nodes[g[j].gr_size] = best_node;
			g[j].gr_size++;
			t[best_node].group = j;
			}
			
		}
	}
}

void assign_groups(node_t t, int **A_matrix, int nodes, int ngroups, double *eigenvector, int centlen, grupa_g g, double **L_matrix, int max_nodes){
	//double *root_val = malloc(ngroups * sizeof(double)); // wartosci wlasne grup od ktorych zaczynamy przydzial do grup;
	eigen_centyl(eigenvector, ngroups, nodes, t, g, L_matrix);

	max_nodes--;

	find_smallest_con(t, g, A_matrix ,nodes, ngroups, max_nodes);

//	ingr(t,nodes,ngroups,root_val);

//	first_con();

}
