#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
 #include "config.h"

int main() {
    /* num_rows, num_comuns,num_bombs, num_bandits */
	unsigned quants[4] = {6, 6, 8, 1};
    /* filler, player, goal, bomb, bandit */
	unsigned char symbols[5] = {'-', 'I', 'x', 'o', 'B'};
    /* first position = player, rest are bandits */
	struct matrix *position;
   	struct matrix *matrix;
	int distance;
	graph_t graph = graph_new();

	srand((unsigned) time(NULL));

	position = malloc(sizeof(struct matrix) * quants[3]+1);

	if (position == NULL) {
		puts("Trouble allocating memory with malloc, exiting....");
		return -1;
	}
	matrix = matrix_new(quants[0],quants[1]);
	matrix_initialize(matrix,symbols[0]);
    start_positions(matrix,position,quants,symbols);
	matrix_print(matrix);
	graph_populate(graph,matrix,symbols);
	distance = graph_shortest_path(graph);
	printf("%d koraka",distance);

	getchar();
	getchar();
	return 0;
}