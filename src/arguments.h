#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <sys/types.h>

int parse_arguments(const int argc, char **args, int64_t *p_A, int64_t *p_B,
                    int8_t *p_W, char **p_mode, char **p_partition,
                    char **p_filename);

#endif
