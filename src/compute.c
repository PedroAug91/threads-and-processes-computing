#include "files.h"
#include "compute.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <threads.h>
#include <time.h>

double sequential_compute(const u_int64_t _start, const u_int64_t _end) {
	clock_t t_start = clock();

	for (u_int64_t i = _start; i <= _end; i++) {
		u_int64_t n = i;
		u_int64_t steps = 0;
		while (n != 1) {
			steps++;
			if (n % 2 == 0) {
				n = n / 2;
			} else {
				n = n * 3 + 1;
			}
		}
		printf("s(%lu) = %lu\n", i, steps);
	}
	clock_t t_end = clock();
	return (double)(t_end - t_start) * 1000 / CLOCKS_PER_SEC;
}

int compute(Arguments *p_args) { 
	// { total_elapsed, min_child_elapsed, max_child_elapsed, agreggation_elapsed }
	ElapsedTimes times = { -1, -1, -1, -1 };

	if (p_args->W == 1) {
		times.total_elapsed = sequential_compute(p_args->A, p_args->B);
	} else {
		printf("WIP\n");
		exit(EXIT_FAILURE);
	}

	return save_results(p_args, &times);

}
