#ifndef COMPUTE_H
#define COMPUTE_H

#include <stdint.h>

int8_t compute(const int64_t start, const int64_t end, const int64_t W,
               const char *partition_type, const char *processing_mode, const char *output_file);
#endif
