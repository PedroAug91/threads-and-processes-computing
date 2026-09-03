#include "arguments.h"
#include "compute.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char **argv) {
	Arguments args = {0, 0, 0, "", "", ""};

	if (parse_arguments(argc, argv, &args) == EXIT_FAILURE) {
		printf("Exiting with failure code: %d...\n", EXIT_FAILURE);
		exit(EXIT_FAILURE);
	}

	if (compute(&args) == EXIT_FAILURE) {
		printf("Exiting with failure code: %d...\n", EXIT_FAILURE);
		exit(EXIT_FAILURE);
	};

	printf("Exiting with success code: %d...\n", EXIT_SUCCESS);
	return EXIT_SUCCESS;
}
