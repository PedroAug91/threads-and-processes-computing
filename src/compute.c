#define _POSIX_C_SOURCE 200809L

#include "compute.h"
#include "arguments.h"
#include "files.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>

typedef struct {
	Arguments *p_args;
	u_int64_t start;
	u_int64_t end;
	u_int64_t step;
	double *p_elapsed;
	u_int64_t *p_steps;
} WorkerCtx;

double now_seconds(void);
u_int64_t collatz_steps(u_int64_t n);
void *worker(void *arg);
void thread_compute(Arguments *p_args, ElapsedTimes *p_t);
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
		return save_results(p_args, &times);
	}

	return EXIT_FAILURE;
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
	u_int64_t L = p_args->B - p_args->A + 1;
	pthread_t threads[W];
	WorkerCtx ctxs[W];
	double child_elapsed[W];
	u_int64_t child_steps[W];

	// 1 2 3 4  ->  START = 1, END = 4, W = 2, L = 4
	// bloco  :  w0 [1,2]  w1 [3,4]
	// ciclico:  w0 {1,3}  w1 {2,4}
	for (uint8_t i = 0; i < W; i++) {
		ctxs[i].p_args = p_args;
		ctxs[i].p_elapsed = &child_elapsed[i];
		ctxs[i].p_steps = &child_steps[i];

		if (strcmp(p_args->partition_mode, "bloco") == 0) {
			u_int64_t base = L / W;
			u_int64_t rem = L % W;
			ctxs[i].start = p_args->A + i * base + (i < rem ? i : rem);
			ctxs[i].end = ctxs[i].start + base - 1 + (i < rem ? 1 : 0);
			ctxs[i].step = 1;
		} else {
			// ciclico: worker w handles n in [A,B] with (n - A) % W == w
			ctxs[i].start = p_args->A + i;
			ctxs[i].end = p_args->B;
			ctxs[i].step = W;
		}

		if (ctxs[i].start > p_args->B) {
			ctxs[i].start = 1;
			ctxs[i].end = 0; // empty range
		}
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
