#include "arguments.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>

int parse_arguments(const int argc, char **args, u_int64_t *p_A, u_int64_t *p_B,
		u_int8_t *p_W, char **p_mode, char **p_partition,
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
	args++;
	if ((argc - 1) != 6) {
		printf("Argument count MUST be 6.\n");
		return 0;
	}

	*p_A = (u_int64_t)atoi(*args++);
	*p_B = (u_int64_t)atoi(*args++);
	*p_W = (u_int8_t)atoi(*args++);
	*p_mode = *args++;
	*p_partition = *args++;
	*p_filename = *args;

	if (*p_A >= *p_B) {
		printf("\"A\" MUST be lesser than \"B\".\n");
		return 0;
	}

	if (*p_W < 1) {
		printf("\"W\" MUST be greater than or equal to \"1\".");
		return 0;
	}

	if (strcmp(*p_mode, "thread") != 0 && strcmp(*p_mode, "processo") != 0) {
		printf("\"modo\" MUST be \"thread\" or \"processo\", got: \"%s\" intead", *p_mode);
		return 0;
	}


	if (strcmp(*p_partition, "bloco") != 0 && strcmp(*p_partition, "ciclo") != 0) {
		printf("\"particao\" MUST be \"bloco\" or \"ciclo\", got: \"%s\" instead", *p_partition);
		return 0;
	}


	return 1;
}
