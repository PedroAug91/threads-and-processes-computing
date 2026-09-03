#include "files.h"
#include "compute.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <threads.h>
#include <time.h>


void sequential_compute(const u_int64_t _start, const u_int64_t _end, ElapsedTimes *p_t) {
	u_int64_t steps = 0;
	clock_t t_start = clock();

	for (u_int64_t i = _start; i <= _end; i++) {
		u_int64_t n = i;
		// printf("Total steps for '%lu':", n);
		steps = 0;
		while (n != 1) {
			steps++;
			if (n % 2 == 0) {
				n = n / 2;
			} else {
				n = n * 3 + 1;
			}
		}
		// printf(" %lu\n", steps);
	}
	clock_t t_end = clock();


	p_t->total_elapsed = (double)(t_end - t_start) * 1000 / CLOCKS_PER_SEC;
}

int compute(Arguments *p_args) { 
	ElapsedTimes times = {
		-1,
		-1,
		-1,
		-1
	};

	if (p_args->W == 1) {
		sequential_compute(p_args->A, p_args->B, &times);
	} else {
		printf("WIP\n");
		exit(EXIT_FAILURE);
	}

	return save_on_file(p_args, &times);

}
