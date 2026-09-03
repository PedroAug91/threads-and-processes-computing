#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <sys/types.h>

typedef struct {
	u_int64_t A;
	u_int64_t B;
	u_int8_t W;
	char *partition_mode;
	char *processing_mode;
	char *output_file;
} Arguments;

int parse_arguments(const int argc, char **args, Arguments *p_args);

#endif
