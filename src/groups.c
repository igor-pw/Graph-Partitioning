#include "headers/groups.h"
#include <limits.h>


//zliczanie ilosci wierzcholkow za pomoca dfs-a
int *check_gr_con(node_t *t, grupa_g g, int ngroups, int nodes){
	int *hbs = malloc(nodes*sizeof(int));
	int *res = malloc(ngroups*sizeof(int));
	for(int i =0; i<ngroups; i++){
		//zerujemy tablice hbs
		for(int j = 0; j< nodes; j++){
			hbs[j] = 0;
		}
		int node = -1;
		//szukamy wierzcholka nalezacego do grupy
		for(int j=0; j < g[i].gr_size && node == -1; j++){
			if(t[g[i].gr_nodes[j]]->group == i){
				node = g[i].gr_nodes[j];
			}
		}
		
		//liczymy ilosc wierzcholkow do ktorych mozemy przejsc z wybranego wierzcholka
		if(node != -1)	
			res[i]=dfs(t,node,i,hbs);
	}
	//zwalniamy pamiec tablicy odwiedzonych wierzcholkow
	free(hbs);
	return res;
}

//dodanie sasiadujacych wierzcholkow 
void ad_nb_nodes(node_t *t, double **L_matrix, int nodes){
	//przechodzmiy po wszystkich wierzcholkoach
	for(int i = 0; i< nodes; i++){
		//zajmujemy pamiec dla ilosci sasiadow i zapisujemy ich ilosc
		t[i]->connected = malloc(L_matrix[i][i] * sizeof(int));
		t[i]->con_count = L_matrix[i][i];
		int tmp = 0;
		//przechodzmi po macierzy i sprawdzamy gdzie wierzcholek ma polaczenie
		for(int j =0; j< nodes; j++){
			if(L_matrix[i][j] == -1){
				t[i]->connected[tmp] = j;
				tmp++;
			}

		}
	}
}

//dodanie wierzchollka do konca kolejki
void add_to_end(que_list **head, int con){
	//alokujemy pamiec na nowy elemnt kolejki
	que_list *new_node = malloc(sizeof(que_list));

	//doodajemy wierzcholek i ustawiamy wskaznik na NULL dla nastepnego elemntu
	new_node->que = con;
	new_node->next = NULL;
	
	//jezeli kolejka byla pusta to ustawiamy pierwszy elemnt jako ostatni i wskazuje na samego siebie
	if(*head == NULL){
		new_node->last = new_node;
		*head = new_node;
	}
	else{	
		//jezli jest wiecej niz jeden elemnt
		que_list *last_node = (*head)->last;
		//przedostatni elemnt wskazuje na nowy ostatni elemnt jako nastepny
		last_node->next = new_node;
		//pierwszy elemnt wskazuje na nowo utworzony ostatni element
		(*head)->last = new_node;
	}

}

//dodanie sasiadow wierzcholka do kolejki 
void add_to_que(que_list **l_gr, node_t *t, int node, int *D_vector){
	//przechodzimy po wszystkich sasiadach wierzcholka 
	for(int i = 0; i <t[node]->con_count; i++){
		int ing = t[node]->connected[i];
		int tmp = D_vector[ing];
		//dodajmy na odpowiedni koniec kolejki wierzcholek
		add_to_end(&l_gr[tmp],ing);
	}

}

//usuwanie pierwszego elementu z kolejki
void rm_first(que_list **head){
	//sprawdzamy czy kolejka juz nie byla pusta
	if(*head == NULL){
		printf("cos nie tak\n");
		return;
	}
	que_list *first_node = *head;
	//ustawiamy poczatek kolejki na nastepny elemnt 
	*head = first_node->next;
	if(*head == NULL){
	//sprawdzamy czy usunelismy wsyztko z kolejki
	}
	//jezeli jest tylko jeden elemnt w kolejce to dodajmy goo jako ostatni elemnt
	else if(first_node->last == first_node){
		(*head)->last=*head;
	}
	//jezeli jest wiecej niz jeden eemnt w kolejce to nowy pierwszy elemnt wskazuje na koniec kolejki
	else{
		(*head)->last = first_node->last;
	}
	//zwalniamy nieaktualny pierwszy elemnt
	free(first_node);
}

//sprawdzamy czy wierzcholek nadaje sie do dodania do grupy
int is_valid(que_list **head, node_t *t){
	//przechodzimy po kolejce dopuki nie jest pusta
	while(*head != NULL){
		int tmp = (*head)->que;
		//sprawdzamy czy wierzcholek nie zostal przydzielony doo zadnej grupy
		if(t[tmp]->group ==-1){
			//jezeli mozna go przydzielic to usuwamy go z kolejki i zwracmy go
			rm_first(head);
			return tmp;
		}
		//jezeli wierzcholek juz ma jakas grupe to usuwamy go z kolejki
		rm_first(head);
	}
	//jezeli kolejka byla pusta badz nie mozna rzydzielic wierzcholka to zwracamy -1
	return -1;
	
}

void add_from_que(que_list **l_gr, node_t *t,grupa_g g, int gr, int *D_vector){
	int succes = 0;
	//przechodzimy po kolejce tyle razy ile moze byc maksymalnych polanczen
	for(int i = 0; i < g[gr].max_con && succes != 1; i++){
		if(l_gr[i] != NULL){
			int node = is_valid(&l_gr[i],t);
			//jezeli kolejka nie jest pusta to staramy sie dodac wierzcholek do grupy i sprawdzamy czy juz wczesniej nie zostal przydzielony
			if(node != -1){
			//dodajemy wszystkie wierzcholki sasiadow do kolejki
			add_to_que(l_gr,t,node,D_vector);
			//przydzielamy badany wierzcholek do grupy
			t[node]->group = gr;
			g[gr].gr_nodes[g[gr].gr_size] = node;
			g[gr].gr_size++;
			return;	
			}
	
		}
	}
}

//dodanie wolnych wierzcholkow
void con_free_nodes(node_t *t, grupa_g g ,int nodes){
	int fixed =0;
	//dokonujemy zmiany dopuki sa jakies wierzcholki bez grupy
	while(fixed != 1){
		fixed = 1;
		//przechodzimy po wszzystkich wierzcholkach i sprawdzmy czy sa jakies bez grupy
		for(int i =0; i <nodes; i++){
			if(t[i]->group ==-1){
				//jezeli natrafilismy na jakis bez grupy szukamy z jakimi innymi wierzcholkami ma polanczenie i wyberamy ta grupe ktora jest najmniejsza
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
				//jezeli znalezlismy to dodajemy wierzcholek do tej grupy
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

void list_gr_con(node_t *t, grupa_g g, int nodes, int ngroups, int max_gr_size, int *D_vector, double *eigenvector, double **L_matrix){
	eigen_quantile(eigenvector, ngroups, nodes, t, g, L_matrix);
	que_list ***l = malloc(ngroups * sizeof(que_list**));
	int tmp = -1;
	//szukamy najwiekszej liczby polanczen z wierzcholka w calym grafie
	for(int i =0; i < nodes; i++){
		if(D_vector[i] > tmp)
			tmp=D_vector[i];
	}	
	tmp++; // bo nie ma 0 polanczen wiec zaczynamy od 1 
	
	//inicjalizujemy dwu wymiarowa tablice na wskazniki do paczatku listy liniowej
	for(int i = 0; i <ngroups; i++){
		g[i].max_con=tmp;
		l[i]=malloc(tmp*sizeof(que_list*));
		for(int j =0; j<tmp; j++){
			l[i][j]=NULL;
		}
	}
	
	//przechodzimy od korzenia w kazdej grupie i przydzielamy mu nowy wierzcholek
	for(int i =0; i <ngroups; i++){
		
		add_to_que(l[i],t,g[i].gr_nodes[0],D_vector);
	}
	
	//dodajemy wierzcholki do grupy az do momentu osiagniecia najwiekszego marginesu przez grupe
	for(int i =0; i < max_gr_size; i++){
		//rownomiernie rozrastamy kazda grupe
		for(int j = 0; j <ngroups; j++){
				add_from_que(l[j],t,g,j,D_vector);	
		}
	}

	//dodajemy do grup wierzcholki ktore nie zostaly jeszcze przydzielone
	con_free_nodes(t,g,nodes);

	//przechodzimy po kazdym elemencie tablicy i zwalniamy pamiec
	for(int i = 0; i < ngroups; i++){
		for(int j=0; j< tmp; j++){
			while(l[i][j] != NULL){
				//zwalniamy pozostale elemnty w kolejce
				rm_first(&l[i][j]);
			}
			//zwalniamy kolejne elemnty wiersza
			free(l[i][j]);
		}
		//zwalniamy kolejne elemnty kolumny
		free(l[i]);
	}
	//zwalniamy tablice struktury
	free(l);

}


void refine_groups(node_t *node, int n, grupa_g group, int max, int min)
{
	//rafinacja grup

	for(int i = 0; i < n; i++)
	{
		//dla kazdego wierzcholka sprawdzamy kandydata do przeniesenia do innej grupy
		if(node[i]->is_leaf && node[i]->gain < 0 && group[node[i]->group].gr_size > min)
		{
			int group_gain = node[i]->gr_gain;

			//jezeli grupa nie osiagnela maksymalnego rozmiaru przerzucamy do niej wierzcholek
			if(group[group_gain].gr_size < max && group_gain >= 0) 
			{
				//aktualizujemy grupe o nowy wierzcholek
				group[node[i]->group].gr_size--;
				node[i]->group = node[i]->gr_gain;
				node[i]->gr_gain = -1;
				node[i]->gain = 0;
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

void free_group(grupa_g group, int n)
{
	for(int i = 0; i < n; i++)
	{
		free(group[i].gr_nodes);
		free(group[i].no_con);
	}
}
