#include <assert.h>
#include "test.h"
#include "shunting_yard.h"

void assert_double(DOUBLE n1, DOUBLE n2, unsigned int precision)
{
	DOUBLE num1 = floor(pow(10, precision) * n1) / pow(10, precision);
	DOUBLE num2 = floor(pow(10, precision) * n2) / pow(10, precision);
	assert(num1 == num2);
}

int test_cmd(int argc, char *argv[])
{
	char err[ERR_MSG_LEN];

	/* Test proper expressions */
	fprintf(stdout, "[TEST] Sample expressions ... \n");
	assert(evaluate("12.1*3.345*1.44", err) == 58.283280);
	assert(strlen(err) == 0);
	assert(evaluate("(12358*(345+21))/12.1", err) == 373803.96694214875);
	fprintf(stdout, "[TEST] Sample expressions completed \n");

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
	return EXIT_SUCCESS;
}

/* Test running expressions in multi-threading manner*/
int test_cmd_mt(int argc, char *argv[])
{
	/* TODO */
}
