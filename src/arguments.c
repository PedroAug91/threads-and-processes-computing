#include "arguments.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>

int parse_arguments(const int argc, char **args, Arguments *p_args) {
	/*
	 * The expected argument count for the program is exactly 6.
	 *
	 * ./varredor <A> <B> <W> <modo> <particao> <arquivo_saida>
	 *
	 * A				- Start of the interval
	 * B				- End of the interval
	 * W				- Number of threads/processes to use
	 * modo[processing_mode]	- "thread" or "processo"
	 * particao[partition_mode]	- "bloco" or "ciclo"
	 * arquivo_saida[output_file]	- Output file (.csv or .txt)
	 *
	 */
	if (argc - 1 != 6) {
		printf("ERROR: Argument count MUST be 6.\n");
		return EXIT_FAILURE;
	}

	errno = 0;

	char *endptr_a;
	p_args->A = strtoul(args[1], &endptr_a, 10); // 20250031399
	if (*endptr_a != '\0' || errno == ERANGE) {
		printf("ERROR: Invalid or out-of-range numbers for A: '%s'\n", args[1]);
		return EXIT_FAILURE;
	}

	char *endptr_b;
	p_args->B = strtoul(args[2], &endptr_b, 10);
	if (*endptr_b != '\0' || errno == ERANGE) {
		printf("ERROR: Invalid or out-of-range numbers for B: '%s'\n", args[2]);
		return EXIT_FAILURE;
	}

	if (p_args->A > p_args->B) {
		printf("ERROR: 'A' MUST be lesser than 'B'.\n");
		printf("A = %ld B = %ld\n", p_args->A, p_args->B);
		return EXIT_FAILURE;
	}

	p_args->W = (u_int8_t)atoi(args[3]);
	if (p_args->W != 1 && p_args->W != 2 && p_args->W != 4 && p_args->W != 8) {
		printf("ERROR: 'W' MUST be '1', '2', '4' or '8'.\n");
		return EXIT_FAILURE;
	}

	p_args->processing_mode = args[4];
	if (strcmp(p_args->processing_mode, "thread") != 0 && strcmp(p_args->processing_mode, "processo") != 0) {
		printf("ERROR: 'modo' MUST be 'thread' or 'processo', got: '%s' intead\n", p_args->processing_mode);
		return EXIT_FAILURE;
	}


	p_args->partition_mode = args[5];
	if (strcmp(p_args->partition_mode, "bloco") != 0 && strcmp(p_args->partition_mode, "ciclo") != 0) {
		printf("ERROR: 'particao' MUST be 'bloco' or 'ciclo', got: '%s' instead\n", p_args->partition_mode);
		return EXIT_FAILURE;
	}

	p_args->output_file = args[6];

	return EXIT_SUCCESS;
}
