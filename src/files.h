#ifndef FILE_H
#define FILE_H

#include "arguments.h"
#include "compute.h"
#include <stdint.h>

int save_results(const Arguments *p_args, const ElapsedTimes *p_t);
void write_parcial(u_int8_t i, u_int64_t steps, double elapsed);
int read_parcial(u_int8_t i, u_int64_t *p_steps, double *p_elapsed);

#endif
