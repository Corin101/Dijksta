struct matrix {
	unsigned num_rows;
	unsigned num_columns;
	unsigned char *cells;
};

/* alocira novu matricu sa num_rows redaka i num_columns stupaca */
struct matrix *matrix_new(unsigned num_rows, unsigned num_columns);

/* popunjava matricu sa fillerima */
void matrix_initialize(struct matrix *matrix, unsigned char symbol);

/* postavlja pocetnu poziciju za igraca, cilj, bombe, bandite */
void start_positions(struct matrix *matrix, struct matrix *position, unsigned *quants, unsigned char *symbols);

/* dohvaca element u retku row i stupcu column */
unsigned char matrix_get(const struct matrix *matrix, unsigned row, unsigned column);

/* postavlja element u retku row i stupcu column na vrijednost weight */
void matrix_set(struct matrix *matrix, unsigned row, unsigned column, unsigned char weight);

/* ispisuje cijelu matricu na standardni izlaz */
void matrix_print(const struct matrix *matrix);

/* oslobadja memoriju alociranu za matricu */
void matrix_delete(struct matrix *matrix);
