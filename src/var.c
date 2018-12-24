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

#include "var.h"

struct var_s* parse_var_list(const char *var_list)
{
	if (!var_list)
		return 0;

	struct var_s* begin = 0;
	struct var_s* end = 0;

	char *src = (char*)strdup(var_list);
	char *name = strtok(src, ";=");
	while (1)
	{
		if (!name)
			name = strtok(NULL, ";=");

		if (!name)
			break;

		char *value = strtok(NULL, ";=");
		if (!value)
			break;

		struct var_s *var = malloc(sizeof(struct var_s));
		strncpy(var->name, name, MAX_VAR_LEN);
		strncpy(var->value, value, MAX_VAR_VALUE_LEN);
		var->next = 0;

		if (begin && end)
		{
			end->next = var;
			end = var;
		}
		else
		{
			begin = var;
			end = var;
		}

		name = 0;
	}

	free(src);

	return begin;
}

DOUBLE evaluate_with_variables(const char* expression, struct var_s* var_list,
		char* err)
{
	DOUBLE evaluated_val = 0;
	char* expr_dup = (char*)strdup(expression);

	/* Replace and validate variables within the expression */
	struct var_s* var = var_list;
	while (var)
	{
		/*TODO Validate value is a numeric*/
		expr_dup = str_replace(expr_dup, var->name, var->value);
		var = var->next;
	}

	/* Process evaluation */
	evaluated_val = evaluate( expr_dup, err );
	free(expr_dup);

	return evaluated_val;

}

void cleanup_list(struct var_s* var_list)
{
	struct var_s* var = var_list;
	while(var)
	{
		struct var_s *tmp = var;
		var = tmp->next;
		free(tmp);
	}
}

