#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

struct matrix *matrix_new(unsigned num_rows, unsigned num_columns) {

	struct matrix *matrix;

	matrix = malloc(sizeof(struct matrix));
	if (matrix == NULL) {
		puts("Trouble allocating memory with malloc, exiting....");
		return NULL;
	}
	matrix->num_rows = num_rows;
	matrix->num_columns = num_columns;
	matrix->cells = malloc(num_rows * num_columns * sizeof(*matrix->cells));

	return matrix;
}

void matrix_initialize(struct matrix *matrix, unsigned char symbol) {

	unsigned row_index, column_index;

	for (row_index = 0; row_index < matrix->num_rows; ++row_index)
		for (column_index = 0; column_index < matrix->num_columns; ++column_index)
			  matrix_set(matrix, row_index, column_index, symbol);
}

void start_positions(struct matrix *matrix, struct matrix *position, unsigned *quants, unsigned char *symbols){
	 
    unsigned row, col, symb_counter=1,counter=0;
    while(1) {
        while(1) {
            row = rand() % quants[0];
            col = rand() % quants[1];
            if (matrix_get(matrix,row,col) != symbols[0])
                continue;
	        matrix_set(matrix,row,col,symbols[symb_counter]);
            if (symb_counter == 1){
                position[0].cells = &symbols[1];
                position[0].num_rows = row;
				position[0].num_columns = col;
                break;
			}
            if (symb_counter == 2)
			break;

            if (symb_counter == 4) {
                position[counter+1].cells = &symbols[1];
                position[counter+1].num_rows = row;
				position[counter+1].num_columns = col;}

            ++counter;
            if (counter == quants[symb_counter-1])
                break;

			}
        ++symb_counter;
        counter = 0;
        if (symb_counter == 5)
            break;
    }
}

unsigned char matrix_get(const struct matrix *matrix, unsigned row, unsigned column) {
	return *(matrix->cells + row * matrix->num_rows + column);
}

void matrix_set(struct matrix *matrix, unsigned row, unsigned column, unsigned char weight) {
	*(matrix->cells + row * matrix->num_rows + column) = weight;
}

void matrix_print(const struct matrix *matrix) {

	unsigned row_index, column_index;
	puts("\n\n\n\n\n");

	for (row_index = 0; row_index < matrix->num_rows; ++row_index) {
		printf("\t\t");
		for (column_index = 0; column_index < matrix->num_columns; ++column_index)
			printf("%2c ", matrix_get(matrix, row_index, column_index));
		putchar('\n');
	}
}

void matrix_delete(struct matrix *matrix) {
	free(matrix->cells);
	free(matrix);
}