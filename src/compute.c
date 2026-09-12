#define _POSIX_C_SOURCE 200809L

#include "compute.h"
#include "arguments.h"
#include "files.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
	u_int64_t start;
	u_int64_t end;
	u_int64_t step;
	double *p_elapsed;
	u_int64_t *p_steps;
} WorkerCtx;

double now_seconds(void);
u_int64_t collatz_steps(u_int64_t n);
void slice_bounds(Arguments *p_args, u_int8_t i, u_int64_t *start,
		u_int64_t *end, u_int64_t *step);
void *worker(void *arg);
void thread_compute(Arguments *p_args, ElapsedTimes *p_t);
void process_compute(Arguments *p_args, ElapsedTimes *p_t);
double sequential_compute(const u_int64_t _start, const u_int64_t _end);

int compute(Arguments *p_args) {
	// { total_elapsed, min_child_elapsed, max_child_elapsed, agreggation_elapsed }
	ElapsedTimes times = {-1, -1, -1, -1};

	if (p_args->W == 1) {
		times.total_elapsed = sequential_compute(p_args->A, p_args->B);
		return save_results(p_args, &times);
	}

	if (strcmp(p_args->processing_mode, "thread") == 0) {
		thread_compute(p_args, &times);
	} else {
		process_compute(p_args, &times);
	}
	return save_results(p_args, &times);
}

double now_seconds(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

u_int64_t collatz_steps(u_int64_t n) {
	u_int64_t steps = 0;
	while (n != 1) {
		steps++;
		if (n % 2 == 0) {
			n = n / 2;
		} else {
			n = n * 3 + 1;
		}
	}
	return steps;
}

// 1 2 3 4  ->  START = 1, END = 4, W = 2, L = 4
// bloco  :  w0 [1,2]  w1 [3,4]
// ciclico:  w0 {1,3}  w1 {2,4}
void slice_bounds(Arguments *p_args, u_int8_t i, u_int64_t *start,
		u_int64_t *end, u_int64_t *step) {
	u_int64_t L = p_args->B - p_args->A + 1;
	u_int8_t W = p_args->W;

	if (strcmp(p_args->partition_mode, "bloco") == 0) {
		u_int64_t base = L / W;
		u_int64_t rem = L % W;
		*start = p_args->A + i * base + (i < rem ? i : rem);
		*end = *start + base - 1 + (i < rem ? 1 : 0);
		*step = 1;
	} else {
		// ciclico: worker w handles n in [A,B] with (n - A) % W == w
		*start = p_args->A + i;
		*end = p_args->B;
		*step = W;
	}

	if (*start > p_args->B) {
		*start = 1;
		*end = 0; // empty range
	}
}

void *worker(void *arg) {
	WorkerCtx *ctx = arg;
	double t0 = now_seconds();

	u_int64_t count = 0;
	for (u_int64_t n = ctx->start; n <= ctx->end; n += ctx->step) {
		count += collatz_steps(n);
	}

	double t1 = now_seconds();
	*(ctx->p_elapsed) = t1 - t0;
	*(ctx->p_steps) = count;
	return NULL;
}

void thread_compute(Arguments *p_args, ElapsedTimes *p_t) {
	u_int8_t W = p_args->W;
	pthread_t threads[W];
	WorkerCtx ctxs[W];
	double child_elapsed[W];
	u_int64_t child_steps[W];

	for (uint8_t i = 0; i < W; i++) {
		ctxs[i].p_elapsed = &child_elapsed[i];
		ctxs[i].p_steps = &child_steps[i];
		slice_bounds(p_args, i, &ctxs[i].start, &ctxs[i].end, &ctxs[i].step);
	}

	double t0 = now_seconds();

	for (uint8_t i = 0; i < W; i++) {
		pthread_create(&threads[i], NULL, worker, &ctxs[i]);
	}

	for (uint8_t i = 0; i < W; i++) {
		pthread_join(threads[i], NULL);
	}

	// Aggregation: parent only reads child slots after all joins.
	double t_agg = now_seconds();

	u_int64_t total_steps = 0;
	double t_min = child_elapsed[0];
	double t_max = child_elapsed[0];
	for (uint8_t i = 0; i < W; i++) {
		total_steps += child_steps[i];
		if (child_elapsed[i] < t_min) {
			t_min = child_elapsed[i];
		}
		if (child_elapsed[i] > t_max) {
			t_max = child_elapsed[i];
		}
	}
	(void)total_steps;

	double t_end = now_seconds();

	p_t->total_elapsed = t_end - t0;
	p_t->min_child_elapsed = t_min;
	p_t->max_child_elapsed = t_max;
	p_t->agreggation_elapsed = t_end - t_agg;
}

void process_compute(Arguments *p_args, ElapsedTimes *p_t) {
	u_int8_t W = p_args->W;
	pid_t pids[W];
	double child_elapsed[W];
	u_int64_t child_steps[W];

	for (uint8_t i = 0; i < W; i++) {
		child_elapsed[i] = 0.0;
		child_steps[i] = 0;
	}

	double t0 = now_seconds();

	for (uint8_t i = 0; i < W; i++) {
		u_int64_t start, end, step;
		slice_bounds(p_args, i, &start, &end, &step);

		pids[i] = fork();
		if (pids[i] < 0) {
			printf("ERROR: fork failed\n");
			exit(EXIT_FAILURE);
		}
		if (pids[i] == 0) {
			// Child: measure only its own useful work, then write its own file.
			double c0 = now_seconds();
			u_int64_t count = 0;
			for (u_int64_t n = start; n <= end; n += step) {
				count += collatz_steps(n);
			}
			double c1 = now_seconds();
			write_parcial(i, count, c1 - c0);
			_exit(EXIT_SUCCESS);
		}
	}

	for (uint8_t i = 0; i < W; i++) {
		waitpid(pids[i], NULL, 0);
	}

	// Aggregation: parent reads child files only after all waits.
	double t_agg = now_seconds();

	u_int64_t total_steps = 0;
	for (uint8_t i = 0; i < W; i++) {
		if (read_parcial(i, &child_steps[i], &child_elapsed[i]) != EXIT_SUCCESS) {
			printf("ERROR: could not read results from parcial_%u.txt\n", i);
		}
		total_steps += child_steps[i];
	}
	(void)total_steps;

	double t_min = child_elapsed[0];
	double t_max = child_elapsed[0];
	for (uint8_t i = 1; i < W; i++) {
		if (child_elapsed[i] < t_min) {
			t_min = child_elapsed[i];
		}
		if (child_elapsed[i] > t_max) {
			t_max = child_elapsed[i];
		}
	}

	double t_end = now_seconds();

	p_t->total_elapsed = t_end - t0;
	p_t->min_child_elapsed = t_min;
	p_t->max_child_elapsed = t_max;
	p_t->agreggation_elapsed = t_end - t_agg;
}

double sequential_compute(const u_int64_t _start, const u_int64_t _end) {
	double t_start = now_seconds();

	u_int64_t count = 0;
	for (u_int64_t i = _start; i <= _end; i++) {
		count += collatz_steps(i);
		// printf("s(%lu) = %lu\n", i, steps);
	}
	(void)count;

	return now_seconds() - t_start;
}