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

#include <assert.h>
#include "test.h"
#include "var.h"

#include <unistd.h>
#include <pthread.h>

void *evaluate_with_variables_mt(void *expr)
{
	sleep(1);

	char err[ERR_MSG_LEN];
	DOUBLE *result_val = malloc(sizeof(DOUBLE));
	*result_val = evaluate_with_variables((char*) expr, 0, err);

	pthread_exit((void*) result_val);
}

/* Test running expressions in multi-thread manner*/
int test_cmd_mt()
{
	pthread_t thread_id_1;
	const char *expr_1 = "5.5*1.44-12";
	pthread_create(&thread_id_1, NULL, evaluate_with_variables_mt,
			(void*) expr_1);

	pthread_t thread_id_2;
	const char *expr_2 = "(8%3)^10";
	pthread_create(&thread_id_2, NULL, evaluate_with_variables_mt,
			(void*) expr_2);

	pthread_t thread_id_3;
	const char *expr_3 = "+12*(345+67)/89.1";
	pthread_create(&thread_id_3, NULL, evaluate_with_variables_mt,
			(void*) expr_3);

	DOUBLE *result = 0;
	pthread_join(thread_id_1, (void*) &result);
	assert(*result == -4.08);
	free(result);

	DOUBLE *result_2 = 0;
	pthread_join(thread_id_2, (void*) &result_2);
	assert(*result_2 == 1024);
	free(result_2);

	DOUBLE *result_3 = 0;
	pthread_join(thread_id_3, (void*) &result_3);
	assert(*result_3 == DBL_MIN);
	free(result_3);
}

void assert_double(DOUBLE n1, DOUBLE n2, unsigned int precision)
{
	DOUBLE num1 = floor(pow(10, precision) * n1) / pow(10, precision);
	DOUBLE num2 = floor(pow(10, precision) * n2) / pow(10, precision);
	assert(num1 == num2);
}

int test_cmd(int argc, char *argv[])
{
	char err[ERR_MSG_LEN];
	DOUBLE result = 0;

	/****************************************************************/
	/* Test proper expressions */
	fprintf(stdout, "[TEST] Sample expressions ... \n");
	assert(evaluate("12.1*3.345*1.44", err) == 58.283280);
	assert(strlen(err) == 0);
	assert(evaluate("(12358*(345+21))/12.1", err) == 373803.96694214875);
	fprintf(stdout, "[TEST] Sample expressions completed \n");

	/****************************************************************/
	/* Test wrong or incomplete input */
	fprintf(stdout, "[TEST] Failure expressions ... \n");

	assert(evaluate("incomplete5+4",err) == DBL_MIN);
	assert(strcmp(err, "Syntax error at 0 position") == 0);
	/* Missing bracket */
	assert(evaluate("(12358*(345+21)/12.1",err) == DBL_MIN);
	/* non-digit first char */
	assert(evaluate("(A12*(345+67)/89.1",err) == DBL_MIN);
	assert(strcmp(err, "Syntax error at 1 position") == 0);
	/* invalid sign */
	assert(evaluate("(+12*(345+67)/89.1",err) == DBL_MIN);
	assert(
			strcmp(err, "Illegal use of binary operator (+) at 1 position")
					== 0);

	fprintf(stdout, "[TEST] Failure expressions completed \n");

	/****************************************************************/
	/* Advanced expressions */
	fprintf(stdout, "[TEST] Advanced expressions ... \n");
	assert(evaluate("5.5*1.44-12", err) == -4.08);
	assert(strlen(err) == 0);
	/* modulo and power operations */
	assert(evaluate("(8%3)^10", err) == 1024);
	/* first char is a sign */
	assert(evaluate("-1.1+(8%3)^10", err) == 1022.90);
	assert_double(evaluate("4   / 5.1", err), 0.7843137255, 8);
	/* float modulo operation */
	assert_double(evaluate("89.2%3", err), 2.2, 2);

	fprintf(stdout, "[TEST] Advanced expressions completed \n");

	/****************************************************************/
	/* Variable expressions */
	fprintf(stdout, "[TEST] Variable expressions ... \n");

	struct var_s *begin = parse_var_list("length=12;width=88");
	result = evaluate_with_variables("(length+width)*2", begin, err);
	assert_double(result, 200, 1);
	cleanup_list(begin);

	begin = parse_var_list("length=12.34;width=88.78;area=33.55");
	result = evaluate_with_variables("(12%5)^3-(length+width)*2.45-area+12%5",
			begin, err);
	assert_double(result, -271.2940f, 5);

	cleanup_list(begin);

	fprintf(stdout, "[TEST] Variable expressions completed \n");

	/****************************************************************/
	/* Multi-threading examples*/
	test_cmd_mt();

	return EXIT_SUCCESS;
}

