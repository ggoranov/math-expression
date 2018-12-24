/*
* MIT License
*
* Copyright (c) 2018 Georgi Goranov
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "var.h"
#include "test.h"
#define ERR_MSG_LEN 256

int evaluate_cmd(int argc, char *argv[])
{
	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s <EVAL_MODE> <expression> <variables list>\n", argv[0]);
		return EXIT_FAILURE;
	}

	struct var_s *begin = 0;
	if (argc >= 4)
		begin = parse_var_list(argv[3]);

	char err[ERR_MSG_LEN];
	DOUBLE result = evaluate_with_variables(argv[2], begin, err);

	cleanup_list(begin);

	if (result != DBL_MIN)
		printf("Result: %.10f\n", result);
	else
	{
		printf("Error: %s\n", err);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

#define CMD_COUNT 2
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
	for (; i < CMD_COUNT; i++)
		if (strcmp(cmds[i].name, argv[1]) == 0)
			return cmds[i].process(argc, argv);

	return EXIT_FAILURE;
}
