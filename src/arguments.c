#include <stdio.h>
#include <stdlib.h>
#include "arguments.h"

char ** parse_arguments(const int argc, char ** args) {
	/*
	 * The expected argument count for the program is exactly 6
	 *
	 * ./varredor <A> <B> <W> <modo> <particao> <arquivo_saida>
	 *
	 * A - Start of the interval
	 * B - End of the interval
	 * W - Number of threads/processes to use
	 * modo - "thread" or "processo"
	 * particao - "bloco" or "ciclo"
	 * arquivo_saida - Output file (.csv or .txt)
	 *
	 */
	if (argc != 6) {
		return NULL;
	}

	char ** p_args = args;

	char * A = *args++;
	char * B = *args++;
	char * W = *args++;

	printf("A = %s B = %s W = %s\n", A, B, W);

	return p_args;
}
