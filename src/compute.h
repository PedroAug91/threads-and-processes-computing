#ifndef COMPUTE_H
#define COMPUTE_H

#include <stdint.h>

int8_t compute(const int64_t _start, const int64_t _end, const int64_t W,
               const char *partition_type, const char *processing_mode);
#endif
