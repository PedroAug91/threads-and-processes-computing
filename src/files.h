#ifndef FILE_H
#define FILE_H

#include <stdint.h>
int save_on_file(const char *file_name, const char *processing_mode,
                 const char *partition_type, const int8_t W, const int64_t *L,
                 const double total_elapsed, const double min_child_elapsed,
                 const double max_child_elapsed,
                 const double agreggation_elapsed);

#endif
