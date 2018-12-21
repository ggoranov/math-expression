#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "shunting_yard.h"
#include "test.h"
#define ERR_MSG_LEN 256

int evaluate_cmd(int argc, char *argv[])
{
	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s <EVAL_MODE> <expression>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char err[ERR_MSG_LEN];
	DOUBLE result = evaluate(argv[2], err);

	if (result != DBL_MIN)
		printf("Result: %.10f\n", result);
	else
	{

		printf("Error: %s\n", err);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

const unsigned int cmds_num = 2;
struct cmd_s
{
	char name[32];
	int (*process)(int argc, char *argv[]);
}

cmds[] =
{
{ "TEST_MODE", test_cmd },
{ "EVAL_MODE", evaluate_cmd }, };

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <TEST_MODE/EVAL_MODE>\n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Map command to callback*/
	unsigned int i = 0;
	for (; i < cmds_num; i++)
		if (strcmp(cmds[i].name, argv[1]) == 0)
			return cmds[i].process(argc, argv);

	return EXIT_FAILURE;
}
