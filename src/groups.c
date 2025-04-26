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

int dfs(node_t *t, int node, int gr, int *hbs){
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

int *check_gr_con(node_t *t, grupa_g g, int ngroups, int nodes){
	int *hbs = calloc(nodes,sizeof(int));
	int *res = malloc(ngroups*sizeof(int));
	for(int i =0; i<ngroups; i++){
		for(int j = 0; j< nodes; j++){
			hbs[j] = 0;
		}
		res[i]=dfs(t,g[i].gr_nodes[0],i,hbs);
	}
	free(hbs);
	return res;
}

void ad_nb_nodes(node_t *t, double **L_matrix, int nodes){
	for(int i = 0; i< nodes; i++){
		t[i]->connected=malloc(L_matrix[i][i] * sizeof(int));
		t[i]->con_count=L_matrix[i][i];
		int tmp = 0;
		for(int j =0; j< nodes; j++){
			if(L_matrix[i][j] == -1){
				t[i]->connected[tmp] = j;
				tmp++;
			}

		}
	}
}

int con_am(node_t *t, int **A_matrix, int nodes, int node){
	int tmp = 0;
	for(int i = 0; i < nodes; i++){
		if(A_matrix[node][i] != 0 && t[i]->group == -1){
			tmp++;
		}
	}
	return tmp;
}

void find_least_con(node_t *t, int **A_matrix, int nodes, int node, int *best_node, int * low_con){
	for(int i = 0; i < nodes; i++){
		//printf("gr = %d, Macierz = %d, czy ma gr = %d, badany node = %d\n",cur_gr, Macierz_s[node][i], t[i].group, node);
		if(A_matrix[node][i] != 0 && t[i]->group == -1){
			int tmp = con_am(t, A_matrix, nodes, i);
			if(tmp < (*low_con)){
				//printf("tmp = %d, i = %d, node = %d\n",tmp,i,node);
				(*low_con) = tmp;
				(*best_node) = i;
			}
		}
		
	}
}

void print_l(que_list *head, int row, int col) {
    if (head == NULL) {
        printf("Lista [%d][%d] jest pusta\n", row, col);
        return;
    }

    printf("Lista [%d][%d]: ", row, col);
    que_list *current = head;
    while (current != NULL) {
        printf("%d -> ", current->que);
        current = current->next;
    }
    printf("NULL\n");

    printf("Ostatni element: %d\n", head->last->que);
}

void add_to_end(que_list **head, int con){
	que_list *new_node = malloc(sizeof(que_list));
		if(new_node == NULL)
			printf("ojoj\n");
	new_node->que = con;
	new_node->next = NULL;

	if(*head == NULL){
		new_node->last = new_node;
		*head = new_node;
	}
	else{
		que_list *last_node = (*head)->last;
		if(last_node == NULL){
			printf("UWAGA!!!\n");
			last_node = *head;
			while(last_node->next != NULL)
				last_node = last_node->next;
			(*head)->last=last_node;
		}
		last_node->next = new_node;
		(*head)->last = new_node;
	}
}


void add_to_que(que_list **l_gr, node_t *t, int node, int *D_vector){
	for(int i = 0; i <t[node]->con_count; i++){
		int ing = t[node]->connected[i];
		int tmp = D_vector[ing];
		//printf("tmp = %d, ing = %d\n",tmp,ing);
		add_to_end(&l_gr[tmp],ing);
	}

}

void rm_first(que_list **head){
	if(*head == NULL){
		printf("cos nie tak\n");
		return;
	}
	que_list *first_node = *head;
	*head = first_node->next;
	if(*head == NULL){
	}
	else if(first_node->last == first_node){
		(*head)->last=*head;
	}
	else{
		(*head)->last = first_node->last;
	}
}

int is_valid(que_list **head, node_t *t, int gr){
	while(*head != NULL){
		int tmp = (*head)->que;
		if(t[tmp]->group ==-1){
			t[tmp]->group = gr;
			rm_first(head);
			return tmp;
		}
		rm_first(head);
	}
	return -1;
	
}

void add_from_que(que_list **l_gr, node_t *t,grupa_g g, int gr, int *D_vector){
	int succes = 0;
	for(int i = 0; i < g[gr].max_con && succes != 1; i++){
		if(l_gr[i] != NULL){
			int node = is_valid(&l_gr[i],t,gr);
		
			if(node != -1){
			add_to_que(l_gr,t,node,D_vector);
			t[node]->group = gr;
			g[gr].gr_nodes[g[gr].gr_size] = node;
			g[gr].gr_size++;
			return;		
			}
	
		}
	}
}

void con_free_nodes(node_t *t, grupa_g g ,int nodes, int max){
	int fixed =0;
	while(fixed != 1){
		fixed = 1;
		for(int i =0; i <nodes; i++){
			if(t[i]->group ==-1){
				int smallest_gr = INT_MAX;
				int gr_to = -1;
				for(int j =0; j< t[i]->con_count; j++){
					int node = t[i]->connected[j];
					int gr = t[node]->group;
					if(gr != -1 && g[gr].gr_size < smallest_gr){ 
						smallest_gr = g[gr].gr_size;
					gr_to = gr;
					}
				}
				if(gr_to !=-1){
				t[i]->group = gr_to;
				fixed = 0;
				g[gr_to].gr_nodes[g[gr_to].gr_size] = i;
				g[gr_to].gr_size++;
				}
			}
		}
	}
	
}

void list_gr_con(node_t *t, grupa_g g, int nodes, int ngroups, int max_gr_size, int min_gr_size, int *D_vector, double *eigenvector, double **L_matrix){
	eigen_centyl(eigenvector, ngroups, nodes, t, g, L_matrix);
	que_list ***l = malloc(ngroups * sizeof(que_list**));
	int tmp = -1;
	for(int i =0; i < nodes; i++){
	if(D_vector[i] > tmp)
		tmp=D_vector[i];
	}	
	tmp++; // bo nie ma 0 polonczen wiec zaczynamy od 1 
	for(int i = 0; i <ngroups; i++){
		
		//printf("nodes = %d, D = %d\n",g[i].gr_nodes[i],D_vector[0]);
		g[i].max_con=tmp;
		l[i]=malloc(tmp*sizeof(que_list*));
		for(int j =0; j<tmp; j++){
			l[i][j]=NULL;
		}
	}


	for(int i =0; i <ngroups; i++){
		
		add_to_que(l[i],t,g[i].gr_nodes[0],D_vector);
	}

	for(int i =0; i < max_gr_size; i++)
		for(int j = 0; j <ngroups; j++)
				add_from_que(l[j],t,g,j,D_vector);	
	
	con_free_nodes(t,g,nodes, max_gr_size);

	for (int i = 0; i < ngroups; i++) {
        	for (int j = 0; j < g[i].max_con; j++) {
            		if (l[i][j] != NULL) {
                		//print_l(l[i][j], i, j);
            		}
        	}
    	}

	/*for(int i =0; i <ngroups; i++){
		printf("gr %d - ",i);
		for(int j =0; j < t[g[i].gr_nodes[0]]->con_count; j++){
			printf("%d ,",t[g[i].gr_nodes[0]]->connected[j]);
		}
		printf("\n");
	}*/
	//t[g[0].gr_nodes[1]]->group=-1;


}
void find_smallest_con(node_t *t, grupa_g g, int **A_matrix, int nodes, int ngroups, int max_gr_size){
	

	for(int i =0; i<max_gr_size; i++){
		for(int j =0 ; j<ngroups; j++){
			int best_node = INT_MAX;
			int low_con = INT_MAX;
			for(int k = 0; k < g[j].gr_size; k++){
				//printf("gr = %d, wiesz = %d\n",j,k);
				if(g[j].no_con[k] == 0){
				find_least_con(t, A_matrix, nodes, g[j].gr_nodes[k], &best_node, &low_con);
					if(low_con == INT_MAX)
						g[j].no_con[k] = 1;
				}
			}
			//printf("best node = %d, low con = %d\n",best_node,low_con);
			if(best_node != INT_MAX && low_con != INT_MAX){
			g[j].gr_nodes[g[j].gr_size] = best_node;
			g[j].gr_size++;
			t[best_node]->group = j;
			}
			
		}
	}
}

void assign_groups(node_t *t, int **A_matrix, int nodes, int ngroups, double *eigenvector, grupa_g g, double **L_matrix, int max_nodes){
	//double *root_val = malloc(ngroups * sizeof(double)); // wartosci wlasne grup od ktorych zaczynamy przydzial do grup;
	eigen_centyl(eigenvector, ngroups, nodes, t, g, L_matrix);

	max_nodes--;

	find_smallest_con(t, g, A_matrix ,nodes, ngroups, max_nodes);


//	ingr(t,nodes,ngroups,root_val);

//	first_con();

}

void refine_groups(node_t *node, int n, grupa_g group, int max, int min)
{
	for(int i = 0; i < n; i++)
	{
		if(node[i]->is_leaf && node[i]->gain < 0 && group[node[i]->group].gr_size > min)
		{
			int group_gain = node[i]->gr_gain;

			if(group[group_gain].gr_size < max && group_gain >= 0) 
			{
				group[node[i]->group].gr_size--;
				node[i]->group = node[i]->gr_gain;
				node[i]->gr_gain = -1;
				group[group_gain].gr_size++;	
			}
		}
	}
}

bool is_in_margin(grupa_g group, int n, int min, int max)
{
	for(int i = 0; i < n; i++)
	{
		if(group[i].gr_size < min || group[i].gr_size > max)
			return false;
	}

	return true;

}
