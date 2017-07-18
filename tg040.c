#include <stdio.h>
#include <stdlib.h>



#define AERO 1
#define ESTRADA 0
#define TRUE 1
#define FALSE 0

typedef struct Edge{
	
	int v1;
	int v2;
	short int peso;
	short int tipo;
	
}*Item;

typedef struct vertice {
	struct vertice *parent;
	short int rank;

} *vertex;



vertex Find_Set(vertex vertice);
void Link(vertex vertice1, vertex vertice2);
void Union(vertex vertice1, vertex vertice2);

void Kruskal(vertex* vertices_list, Item* edge_list, int size_of_edge_list);



Item new_item(int v1, int v2, int peso, int tipo);


int less(const void* a, const void* b);



int temp_aero_cost = 0;
int count_aero = 0;
int count_estrada = 0;
int custo_total = 0;
int nr_merges = 0;
int custo_total_estrada = -1;
int temp_count_estrada = 0;
int temp_nr_merges = 0;
short int falhou_aero = FALSE;

int nr_cidades = 0;
int nr_aeroportos = 0;
int nr_estradas = 0;

int main() {

	int i;
	int cidade;
	int cidade2;
	int peso;

	if(!scanf("%d", &nr_cidades)) return -1; 

	vertex *vertices_list = (vertex*) malloc(sizeof(vertex)*(nr_cidades+1));
	
	
	for (i = 0; i <= nr_cidades ; ++i)
	{
		vertices_list[i] = (vertex) malloc(sizeof(struct vertice));
		vertices_list[i]->parent = vertices_list[i];
		vertices_list[i]->rank = 0;
	}


	if(!scanf("%d", &nr_aeroportos)) return -1; 

	Item *temp_edge_list = (Item*) malloc(sizeof(Item)*(nr_aeroportos));

	for (i = 0; i < nr_aeroportos; ++i)
	{
		if(!scanf("%d %d", &cidade, &peso)) return -1;
		temp_edge_list[i] = new_item(cidade, 0, peso, AERO);

	}
	
	if(!scanf("%d", &nr_estradas)) return -1; 

	Item *edge_list = (Item*) malloc(sizeof(Item)*(nr_aeroportos + nr_estradas));
	Item *edge_list_estradas = (Item*) malloc(sizeof(Item)*(nr_estradas));

	for (i = 0; i < nr_aeroportos; ++i)
	{
		edge_list[i] = temp_edge_list[i];
	}

	

	free(temp_edge_list);
	


	for (i = 0; i < nr_estradas; ++i)
	{
		if(!scanf("%d %d %d", &cidade, &cidade2, &peso)) return -1;
		Item new = new_item(cidade, cidade2, peso, ESTRADA);
		edge_list[i + nr_aeroportos] = new;
		edge_list_estradas[i] = new;

	}

	
 
   qsort(edge_list, nr_aeroportos+nr_estradas, sizeof(Item), less);
   qsort(edge_list_estradas, nr_estradas, sizeof(Item), less);
	

	Kruskal(vertices_list, edge_list_estradas, nr_estradas);

	custo_total_estrada = custo_total;
	temp_count_estrada = count_estrada;
	temp_nr_merges = nr_merges;
	count_estrada = 0;
	custo_total = 0;
	nr_merges = 0;
	

	for (i = 0; i <= nr_cidades ; ++i)
	{
		vertices_list[i]->parent = vertices_list[i];
		vertices_list[i]->rank = 0;
	}

	Kruskal(vertices_list, edge_list, nr_aeroportos + nr_estradas);

	
	if (count_aero == 1)
	{
		custo_total -= temp_aero_cost;
		--count_aero;
	}

	if (falhou_aero == FALSE)
	{
		if (nr_merges != nr_cidades)
		{
			printf("Insuficiente\n");
		}

		else
		{
			printf("%d\n", custo_total);
			printf("%d %d\n", count_aero, count_estrada );
		}
		
	}

	else
	{
		
		if (temp_nr_merges != (nr_cidades - 1))
		{
			printf("Insuficiente\n");
		}

		else
		{
			printf("%d\n", custo_total);
			printf("0 %d\n", temp_count_estrada );
		}
	}

	

	for (i = 0; i < nr_cidades + 1; ++i)
	{
		free(vertices_list[i]);
	}

	for ( i = 0; i < nr_aeroportos + nr_estradas; ++i)
	{
		free(edge_list[i]);
	}

	
	free(vertices_list);
	free(edge_list);	
	free(edge_list_estradas);

	return 0;
}





vertex Find_Set(vertex vertice) 
{
	if (vertice != vertice->parent)
	{
		vertice->parent = Find_Set(vertice->parent);
	}

	return vertice->parent;
}

void Link(vertex vertice1, vertex vertice2)
{
	if ( vertice1->rank > vertice2->rank )
	{
		vertice2->parent = vertice1;
	}
	
	else
	{
		vertice1->parent = vertice2;
	}

	if ( vertice1->rank == vertice2->rank )
	{
		vertice2->rank = vertice2->rank + 1;
	}
}

void Union(vertex vertice1, vertex vertice2)
{
	Link(Find_Set(vertice1), Find_Set(vertice2));
}


void Kruskal(vertex* vertices_list, Item* edge_list, int size_of_edge_list) 
{
	int i = 0;
	for (i = 0; i < size_of_edge_list ; ++i)
	{
		Item ig = edge_list[i];
		
		if (Find_Set(vertices_list[ig->v1]) != Find_Set(vertices_list[ig->v2]))
		{
			++nr_merges;
			custo_total += ig->peso;
			
			if ((custo_total >= custo_total_estrada) && (custo_total_estrada != -1) && temp_nr_merges == (nr_cidades - 1))
			{
				
				falhou_aero = TRUE;
				return;
			}
			if (ig->tipo == AERO)
			{
				++count_aero;
				if (count_aero == 1)
				{
					temp_aero_cost += ig->peso;
				}
			}
			else 
			{
				++count_estrada;
			}

			Union(vertices_list[ig->v1], vertices_list[ig->v2]);
		}
	}
}







/* -- Arvore Binaria Balanceada -- */



int less(const void* a, const void* b)
{
	Item item1 = *(Item*) a;
	Item item2 = *(Item*) b;

	if (item1->peso < item2->peso || (item1->peso == item2->peso && item1->tipo < item2->tipo))
	{
		return FALSE;
	}

	return TRUE;
}


Item new_item(int v1, int v2, int peso, int tipo) 
{
	Item new = (Item) malloc(sizeof(struct Edge));
		
	new->v1 = v1;
	new->v2 = v2;
	new->peso = peso;
	new->tipo = tipo;

	return new;
}



