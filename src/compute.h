#ifndef COMPUTE_H
#define COMPUTE_H

#include "arguments.h"
#include <stdint.h>

typedef struct {
	double total_elapsed;
	double min_child_elapsed;
	double max_child_elapsed;
	double agreggation_elapsed;
} ElapsedTimes;

int compute(Arguments *p_args);
#endif
