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
