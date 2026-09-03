#include "arguments.h"
#include "compute.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char **argv) {
	int64_t A, B;
	int8_t W;
	char *processing_mode;
	char *partition_type;
	char *output_file;

	if (!parse_arguments(argc, argv, &A, &B, &W, &processing_mode,
				&partition_type, &output_file)) {
		printf("Exiting...\n");
		exit(1);
	}

	if (!compute(A, B, W, partition_type, processing_mode, output_file)) {
		printf("Exiting...\n");
		exit(1);
	};

}
