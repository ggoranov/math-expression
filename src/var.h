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
#include "shunting_yard.h"
#include "string_utils.h"

#define MAX_VAR_LEN 200
#define MAX_VAR_VALUE_LEN 30

/* linked-list */
struct var_s
{
	char name[MAX_VAR_LEN];
	char value[MAX_VAR_VALUE_LEN];
	struct var_s* next;
};

/* Parse a string with variables */
struct var_s* parse_var_list(const char *var_list);
/* TODO Cleanup allocated list*/
void cleanup_list(struct var_s* begin);


/* Evaluate a math expression with any variables
 * E.g 12*2 + length - 10^2 must return length - 76
 * E.g 12*2 + 2*(length - width) must return 2*length - 2*width + 24
 * returns result as double if parsing and calculations do pass successfully
 * returns DBL_MIN if failure occur. err is populated too
 * Thread-safe support
 */
DOUBLE evaluate_with_variables(const char* expression, struct var_s* var_list, char* err);
