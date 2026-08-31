#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define TRUE 1;
#define FALSE 0;

int sequential_compute(const int64_t _start, const int64_t _end, double *p_elapsed) {

	clock_t t_start = clock();
	for (int64_t i = _start; i <= _end; i++) {
		int64_t n = _start;
		while (n != 1) {
			if (n % 2 == 0) {
				n = n / 2;
			} else {
				n = n * 3 + 1;
			}
		}
	}
	clock_t t_end = clock();

	*p_elapsed = (double)(t_end - t_start) / CLOCKS_PER_SEC;

	return TRUE;
}

int8_t compute(const int64_t _start, const int64_t _end, const int64_t W,
		const char *partition_type, const char *processing_mode) {
	int8_t done = 0;

	// milliseconds
	double total_elapsed = -1;
	double min_child_elapsed = -1;
	double max_child_elapsed = -1;
	double agreggation_elapsed = -1;

	if (W == 1) {
		done = sequential_compute(_start, _end, &total_elapsed);
	} else {
		printf("WIP\n");
	}

	int64_t L = _end - _start + 1;
	printf("%s, %s, %lu, %lu, %f, %f, %f, %f\n", processing_mode, partition_type, W, L, total_elapsed, min_child_elapsed, max_child_elapsed, agreggation_elapsed);

	return done;
}
