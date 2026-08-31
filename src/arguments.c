#include "arguments.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>

int parse_arguments(const int argc, char **args, int64_t *p_A, int64_t *p_B,
		int8_t *p_W, char **p_mode, char **p_partition,
		char **p_filename) {
	/*
	 * The expected argument count for the program is exactly 6.
	 *
	 * We subtract 1 due to the 'null terminator' at the end of
	 * the arguments array.
	 *
	 * ./varredor <A> <B> <W> <modo> <particao> <arquivo_saida>
	 *
	 * A - Start of the interval
	 * B - End of the interval
	 * W - Number of threads/processes to use
	 * modo[processing_mode] - "thread" or "processo"
	 * particao[partition_type] - "bloco" or "ciclo"
	 * arquivo_saida[output_file] - Output file (.csv or .txt)
	 *
	 */
	if ((argc - 1) != 6) {
		printf("Argument count MUST be 6.\n");
		return 0;
	}

	errno = 0;

	char *endptr_a;
	*p_A = strtol(args[1], &endptr_a, 10) + 31399; // 20250031399

	if (*endptr_a != '\0' || errno == ERANGE) {
		printf("Invalid or out-of-range numbers for A: '%s'\n", args[1]);
		return 0;
	}

	char *endptr_b;
	*p_B = strtol(args[2], &endptr_b, 10);
	if (*endptr_b != '\0' || errno == ERANGE) {
		printf("Invalid or out-of-range numbers for B: '%s'\n", args[2]);
		return 0;
	}

	*p_W = (int8_t)atoi(args[3]);
	*p_mode = args[4];
	*p_partition = args[5];
	*p_filename = args[6];

	if (*p_A >= *p_B) {
		printf("'A' MUST be lesser than 'B'.\n");
		printf("A=%ld B=%ld\n", *p_A, *p_B);
		return 0;
	}

	if (*p_W != 1 && *p_W != 2 && *p_W != 4 && *p_W != 8) {
		printf("'W' MUST be '1', '2', '4' or '8'.\n");
		return 0;
	}

	if (strcmp(*p_mode, "thread") != 0 && strcmp(*p_mode, "processo") != 0) {
		printf("'modo' MUST be 'thread' or 'processo', got: '%s' intead\n", *p_mode);
		return 0;
	}


	if (strcmp(*p_partition, "bloco") != 0 && strcmp(*p_partition, "ciclo") != 0) {
		printf("'particao' MUST be 'bloco' or 'ciclo', got: '%s' instead\n", *p_partition);
		return 0;
	}


	return 1;
}
