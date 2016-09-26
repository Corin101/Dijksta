#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

typedef struct graph *graph_t;

/* izrada novog grafa */
graph_t graph_new();

/* brisanje grafa */
void graph_delete(graph_t graph);

/* popunjavanje grafa cvorovima */
int graph_populate(graph_t graph, struct matrix *matrix, unsigned char *symbols);

/* postavljanje putanje */
void graph_update_data(graph_t graph, unsigned char *bitmap_data);

/* ispis grafa u konzolu */
void graph_print(graph_t graph, struct matrix *matrix);

/* izracun najkraceg puta */
int graph_shortest_path(graph_t graph);

#endif /* GRAPH_H_INCLUDED */
