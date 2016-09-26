#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "set.h"
#include "config.h"
#include "graph.h"

#define LEFT 0
#define RIGHT 1
#define TOP 2
#define BOTTOM 3
#define NUM_NEIGHBORS 4

struct graph_node {
    struct graph_node *neighbors[NUM_NEIGHBORS]; /* susjedni cvorovi */
    struct graph_node *previous;                 /* prethodni cvor na putanji do zadanog cvora */
    unsigned distance;                           /* udaljenost do zadanog cvora */
    int w, h;                                    /* ZA ISCRTAVANJE: koordinate */
};

typedef struct graph_node *node_t;

node_t node_new(int w, int h) {

    node_t node = (node_t)malloc(sizeof(struct graph_node));

    /* postavi susjede */
    node->neighbors[LEFT] = NULL;
    node->neighbors[RIGHT] = NULL;
    node->neighbors[TOP] = NULL;
    node->neighbors[BOTTOM] = NULL;

    node->previous = NULL;
    node->distance = INT_MAX;
    node->w = w;
    node->h = h;

    return node;
}

struct graph {
    unsigned num_nodes;
    node_t *nodes;
    node_t start;
    node_t end;
};

graph_t graph_new(){

    graph_t graph = (graph_t)malloc(sizeof(struct graph));
    
    graph->num_nodes= 0;
    graph->nodes = NULL;
    graph->start = NULL;
    graph->end = NULL;

    return graph;
}

void graph_delete(graph_t graph){
    
    unsigned index;
    
    for (index = 0; index < graph->num_nodes; index++) {
        free(graph->nodes[index]);
    }

    free(graph->nodes);
}

int graph_populate(graph_t graph, struct matrix *matrix, unsigned char *symbols) {

    size_t h, w;

    graph->num_nodes = matrix->num_columns*matrix->num_rows;
    graph->nodes = malloc(sizeof(node_t) * graph->num_nodes);

    /* obrada piksela */
    for (h = 0; h < matrix->num_rows; h++) {
        for (w = 0; w < matrix->num_columns; w++) {
            unsigned char filler = matrix_get(matrix,h,w);
            node_t node = NULL;

            /* cvorovi postoje tamo gdje nema prepreke*/
            if (filler != symbols[3] && filler != symbols[4]) {
                 /* napravi novi cvor */
                node = node_new(w, h);
                
                /* postavi pocetni cvor */            
                if (filler == symbols[1]) {
                    graph->start = node;
                }
                /* postavi zavrsni cvor */
                else if (filler == symbols[2]) {
                    graph->end = node;
                }
            }
            
            /* postavi cvor u graf */
            graph->nodes[h * matrix->num_columns + w] = node;
        }
    } 

    if (!graph->start) {
        printf("Nije postavljen pocetni cvor");
        return 1;
    }

    if (!graph->end) {
        printf("Nije postavljen zavrsni cvor");
        return 2;
    }

    /* spajanje cvorova */
    for (h = 0; h < matrix->num_rows; h++) {
        for (w = 0; w < matrix->num_columns; w++) {

            node_t node = graph->nodes[h * matrix->num_columns + w];

            if (node == NULL)
                continue;

            /* postavi gornji cvor */
            if (h > 0) {
                node->neighbors[TOP] = graph->nodes[(h - 1) * matrix->num_columns + w];
                if (node->neighbors[TOP]) {
                    /* postavi susjedov donji */
                    node->neighbors[TOP]->neighbors[BOTTOM] = node;
                }
            }
            /* postavi lijevi cvor */
            if (w > 0) {
                node->neighbors[LEFT] = graph->nodes[h * matrix->num_columns + (w - 1)];
                if (node->neighbors[LEFT]) {
                    /* postavi susjedov desni */
                    node->neighbors[LEFT]->neighbors[RIGHT] = node;
                }
            }
        }
    }

    graph_print(graph,matrix);

    return 0;

}


void graph_print(graph_t graph, struct matrix *matrix) {

    unsigned index, node_index;
    const unsigned sketch_width = 2 * matrix->num_columns, sketch_height = 2 * matrix->num_rows;
    char* graph_sketch = (char*)malloc(sketch_width * sketch_height);

    /* popuni cijelu skicu na znak razmak*/
    for (index = 0; index < sketch_width * sketch_height; index++) {
        graph_sketch[index] = ' ';
    }

    /* postavi cvorove i veze */
    for (node_index = 0; node_index < graph->num_nodes; node_index++) {
        size_t center_w, center_h;
        node_t node = graph->nodes[node_index];

        /* preskacemo nepostojece cvorove */
        if (node == NULL)
            continue;

        center_w = 2 * node->w + 1;
        center_h = 2 * node->h + 1;

        if (node == graph->start)
            graph_sketch[sketch_width * center_h + center_w] = 'I';
        else if (node == graph->end)
            graph_sketch[sketch_width * center_h + center_w] = 'x';
        else
            graph_sketch[sketch_width * center_h + center_w] = '+';
        
        if (node->neighbors[TOP])
            graph_sketch[sketch_width * (center_h - 1) + center_w] = '|';
         if (node->neighbors[LEFT])
            graph_sketch[sketch_width * center_h + center_w - 1] = '-';
    }

    /* ispisi skicu */	
	printf("\t\t");
    for (index = 0; index < sketch_width * sketch_height; index++) {
        printf("%c", graph_sketch[index]);
        if ((index+1) % (sketch_width) == 0) {
            printf("\n");
			printf("\t\t");
        }
    }
    free(graph_sketch);
}

int graph_shortest_path(graph_t graph) {
	int cnt;
	node_t temp, next;
	set_t set_vrhova;
	set_iterator_t iter;
	temp = graph->start;
	temp->distance=0;
	set_vrhova = set_new();
	set_add(set_vrhova,temp);
	while(!set_is_empty(set_vrhova)) {

		iter = set_iterator(set_vrhova);
		temp = (node_t)set_iget(iter);
		set_iremove(iter);

		for(cnt = 0;cnt != 4; ++cnt) {
			if (temp->neighbors[cnt] != NULL)
				if (temp->neighbors[cnt]->distance > temp->distance+1) {
					temp->neighbors[cnt]->distance = temp->distance+1;
					next = temp->neighbors[cnt];
					next->previous = temp;
					set_add(set_vrhova,temp->neighbors[cnt]); 
			}
		}

	}
	set_iterator_delete(iter);
	set_delete(set_vrhova);
	return graph->end->distance;
}
