#include "arguments.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char **argv) {
	u_int64_t A, B;
	u_int8_t W;
	char *processing_mode;
	char *partition_type;
	char *output_file;

	u_int64_t *p_A = &A;
	u_int64_t *p_B = &B;
	u_int8_t *p_W = &W;
	char **p_proc_Mode = &processing_mode;
	char **p_partition = &partition_type;
	char **p_output = &output_file;

	if (!parse_arguments(argc, argv, p_A, p_B, p_W, p_proc_Mode, p_partition,
				p_output)) {
		exit(1);
	}

	printf("A=%lu\nB=%lu\nW=%hhu\nmodo=%s\nparticao=%s\narquivo=%s\n", A, B, W, processing_mode, partition_type, output_file);
}
