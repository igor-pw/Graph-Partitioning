#include "headers/groups.h"

//inicjalizacja grup, -1 oznacza ze wieszcholek nie ma przydzielonej zadnej grupy a na koncu przydziale wieszcholki roota
void ingr(node_t t, int nodes, int ngroups, double *root_val){
	for(int i =0; i <nodes; i++)
		t[i].group = -1;

	for( int i= 0; i< ngroups; i++){
		for( int j = 0; j < nodes; j++){
			if(t[j].eigenvalue == root_val[i]){
				t[j].group = i;
				break;
			}
		}	
	}
}

void first_con(node_t t, double **Macierz_s, int nodes, int ngroups){
	double min_dif = -1.0;
	for(int i =0; i < ngroups; i++){
		for(int j =0; j < nodes; j++){
			double tmp = 		
		}
	}
}
		
void assign_groups(node_t t, double **Macierz_s, int nodes, int ngroups, double *eigenvector, int centlen){
	double *root_val = malloc(ngroups * sizeof(double)); // wartosci wlasne grup od ktorych zaczynamy przydzial do grup;
	root_val = eigen_centyl(eigenvector, ngroups, nodes, root_val);

	for(int i = 0; i < ngroups; i ++){
		printf("root value %d = %.15lf\n",i , root_val[i]);
	}
	ingr(t,nodes,ngroups,root_val);

	first_con();

}
