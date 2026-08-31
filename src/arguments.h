#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <sys/types.h>

int parse_arguments(const int argc, char **args, u_int64_t *p_A, u_int64_t *p_B,
                    u_int8_t *p_W, char **p_mode, char **p_partition,
                    char **p_filename);

#endif
