#include "files.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int save_results(const Arguments *p_args, const ElapsedTimes *p_t) {
	FILE *p_file;

	p_file = fopen(p_args->output_file, "a");

	if (p_file == NULL) {
		printf("ERROR: Could not open the file %s\n", p_args->output_file);
		return EXIT_FAILURE;
	}

	u_int64_t L = p_args->B - p_args->A + 1;

	if (p_args->W == 1) {
		fprintf(p_file, "%s, %s, %hhu, %ld, %.2e, -1, -1, -1\n",
				p_args->processing_mode, p_args->partition_mode, p_args->W, L,
				p_t->total_elapsed);
		fclose(p_file);
		return EXIT_SUCCESS;
	}

	fprintf(p_file, "%s, %s, %hhu, %ld, %.2e, %.2e, %.2e, %.2e\n",
			p_args->processing_mode, p_args->partition_mode, p_args->W, L,
			p_t->total_elapsed, p_t->max_child_elapsed, p_t->min_child_elapsed,
			p_t->agreggation_elapsed);

	fclose(p_file);

	return EXIT_SUCCESS;
}

void write_parcial(u_int8_t i, u_int64_t steps, double elapsed) {
	char name[32];
	snprintf(name, sizeof name, "parcial_%u.txt", i);
	FILE *f = fopen(name, "w");
	if (f == NULL) {
		printf("ERROR: could not open %s\n", name);
		return;
	}
	fprintf(f, "%lu\n%.9f\n", steps, elapsed);
	fclose(f);
}

int read_parcial(u_int8_t i, u_int64_t *p_steps, double *p_elapsed) {
	char name[32];
	snprintf(name, sizeof name, "parcial_%u.txt", i);
	FILE *f = fopen(name, "r");
	if (f == NULL) {
		return EXIT_FAILURE;
	}
	int r = fscanf(f, "%lu\n%lf", p_steps, p_elapsed);
	fclose(f);
	return r == 2 ? EXIT_SUCCESS : EXIT_FAILURE;
}
