#include "files.h"
#include <stdio.h>

int save_on_file(const char *file_name, const char *processing_mode,
		const char *partition_type, const int8_t W, const int64_t *L,
		const double total_elapsed, const double min_child_elapsed,
		const double max_child_elapsed,
		const double agreggation_elapsed) {
	FILE *p_file;

	p_file = fopen(file_name, "a");

	if (p_file == NULL) {
		printf("ERROR: Could not open the file %s\n", file_name);
		return 0;
	}

	if (W == 1) {
		fprintf(p_file, "%s, %s, %hhu, %ld, %.2e, -1, -1, -1\n", processing_mode,
				partition_type, W, *L, total_elapsed);
		fclose(p_file);
		return 1;
	}

	fprintf(p_file, "%s, %s, %hhu, %ld, %.2e, %.2e, %.2e, %.2e\n",
			processing_mode, partition_type, W, *L, total_elapsed,
			min_child_elapsed, max_child_elapsed, agreggation_elapsed);
	fclose(p_file);

	return 1;
}
