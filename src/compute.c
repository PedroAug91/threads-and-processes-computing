#include "files.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <threads.h>
#include <time.h>


void sequential_compute(const int64_t _start, const int64_t _end, double *p_elapsed) {
	u_int64_t steps = 0;
	clock_t t_start = clock();

	for (int64_t i = _start; i <= _end; i++) {
		int64_t n = _start;
		steps = 0;
		while (n != 1) {
			steps++;
			if (n % 2 == 0) {
				n = n / 2;
			} else {
				n = n * 3 + 1;
			}
		}
	}
	clock_t t_end = clock();

	printf("Passos = %lu\n", steps);

	*p_elapsed = (double)(t_end - t_start) * 1000 / CLOCKS_PER_SEC;
}

int8_t compute(const int64_t start, const int64_t end, const int64_t W,
		const char *partition_type, const char *processing_mode, const char *output_file) {
	double total_elapsed = -1;
	double min_child_elapsed = -1;
	double max_child_elapsed = -1;
	double agreggation_elapsed = -1;

	if (W == 1) {
		sequential_compute(start, end, &total_elapsed);
	} else {
		printf("WIP\n");
		exit(1);
	}

	int64_t L = end - start + 1;

	return save_on_file(output_file, processing_mode, partition_type, W, &L, total_elapsed, min_child_elapsed, max_child_elapsed, agreggation_elapsed);

}
