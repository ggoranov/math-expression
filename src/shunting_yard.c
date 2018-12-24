#include "shunting_yard.h"

typedef int bool;
#define true 1
#define false 0

DOUBLE eval_sqrt(DOUBLE a1, DOUBLE a2)
{
	return sqrt(a1);
}

DOUBLE eval_uminus(DOUBLE a1, DOUBLE a2)
{
	return -a1;
}
DOUBLE eval_exp(DOUBLE a1, DOUBLE a2)
{
	return a2 < 0 ? 0 : (a2 == 0 ? 1 : a1 * eval_exp(a1, a2 - 1));
}
DOUBLE eval_mul(DOUBLE a1, DOUBLE a2)
{
	return a1 * a2;
}
DOUBLE eval_div(DOUBLE a1, DOUBLE a2)
{
	if (!a2)
	{/* TODO */
		fprintf(stderr, "ERROR: Division by zero\n");
		exit(EXIT_FAILURE);
	}
	return a1 / a2;
}
DOUBLE eval_mod(DOUBLE a1, DOUBLE a2)
{
	if (!a2)
	{
		/* TODO */
		fprintf(stderr, "ERROR: Division by zero\n");
		exit(EXIT_FAILURE);
	}
	return fmod(a1, a2);
}
DOUBLE eval_add(DOUBLE a1, DOUBLE a2)
{
	return a1 + a2;
}
DOUBLE eval_sub(DOUBLE a1, DOUBLE a2)
{
	return a1 - a2;
}

enum
{
	ASSOC_NONE = 0, ASSOC_LEFT, ASSOC_RIGHT
};

struct op_s
{
	char op;
	int prec;
	int assoc;
	int unary;
	DOUBLE (*eval)(DOUBLE a1, DOUBLE a2);
} ops[] =
{
{ 's', 11, ASSOC_LEFT, 0, eval_sqrt },
{ '_', 10, ASSOC_RIGHT, 1, eval_uminus },
{ '^', 9, ASSOC_RIGHT, 0, eval_exp },
{ '*', 8, ASSOC_LEFT, 0, eval_mul },
{ '/', 8, ASSOC_LEFT, 0, eval_div },
{ '%', 8, ASSOC_LEFT, 0, eval_mod },
{ '+', 5, ASSOC_LEFT, 0, eval_add },
{ '-', 5, ASSOC_LEFT, 0, eval_sub },
{ '(', 0, ASSOC_NONE, 0, NULL },
{ ')', 0, ASSOC_NONE, 0, NULL } };

bool issep(const char ch)
{
	return (ch == DECIMAL_SEPARATOR);
}
struct eval_ctx
{
	struct op_s *opstack[MAXOPSTACK];
	unsigned int nopstack;

	DOUBLE numstack[MAXNUMSTACK];
	unsigned int nnumstack;

	char err[ERR_MSG_LEN];
};

struct op_s *getop(char ch)
{
	unsigned int i;
	for (i = 0; i < sizeof ops / sizeof ops[0]; ++i)
	{
		if (ops[i].op == ch)
			return ops + i;
	}
	return NULL;
}

void push_opstack(struct eval_ctx *ctx, struct op_s *op)
{
	if (ctx->nopstack > MAXOPSTACK - 1)
	{
		sprintf(ctx->err, "Operator stack overflow");
		return;
	}
	ctx->opstack[ctx->nopstack++] = op;
}

struct op_s *pop_opstack(struct eval_ctx *ctx)
{
	if (!ctx->nopstack)
	{
		sprintf(ctx->err, "Operator stack empty");
		return NULL;
	}
	return ctx->opstack[--ctx->nopstack];
}

void push_numstack(struct eval_ctx *ctx, DOUBLE num)
{
	if (ctx->nnumstack > MAXNUMSTACK - 1)
	{
		sprintf(ctx->err, "Number stack overflow");
		return;
	}
	ctx->numstack[ctx->nnumstack++] = num;
}

DOUBLE pop_numstack(struct eval_ctx *ctx, char *err)
{
	if (!ctx->nnumstack)
	{
		sprintf(ctx->err, "Number stack empty");
		return DBL_MIN;
	}
	return ctx->numstack[--ctx->nnumstack];
}

bool shunt_op(struct eval_ctx *ctx, struct op_s *op, char *err)
{
	struct op_s *pop;
	DOUBLE n1, n2;
	if (op->op == '(')
	{
		push_opstack(ctx, op);
		return true;
	}
	else if (op->op == ')')
	{
		while (ctx->nopstack > 0 && ctx->opstack[ctx->nopstack - 1]->op != '(')
		{
			pop = pop_opstack(ctx);
			n1 = pop_numstack(ctx, err);
			if (n1 == DBL_MIN)
				return false;

			if (pop->unary)
				push_numstack(ctx, pop->eval(n1, 0));
			else
			{
				n2 = pop_numstack(ctx, err);
				if (n2 == DBL_MIN)
					return false;

				push_numstack(ctx, pop->eval(n2, n1));
			}
		}
		if (!(pop = pop_opstack(ctx)) || pop->op != '(')
		{
			sprintf(err, "Stack error. No matching \'(\'");
			return false;
		}
		return true;
	}

	if (op->assoc == ASSOC_RIGHT)
	{
		while (ctx->nopstack && op->prec < ctx->opstack[ctx->nopstack - 1]->prec)
		{
			pop = pop_opstack(ctx);
			n1 = pop_numstack(ctx, err);
			if (n1 == DBL_MIN)
				return false;

			if (pop->unary)
				push_numstack(ctx, pop->eval(n1, 0));
			else
			{
				n2 = pop_numstack(ctx, err);
				if (n2 == DBL_MIN)
					return false;

				push_numstack(ctx, pop->eval(n2, n1));
			}
		}
	}
	else
	{
		while (ctx->nopstack
				&& op->prec <= ctx->opstack[ctx->nopstack - 1]->prec)
		{
			pop = pop_opstack(ctx);
			n1 = pop_numstack(ctx, err);
			if (n1 == DBL_MIN)
				return false;

			if (pop->unary)
				push_numstack(ctx, pop->eval(n1, 0));
			else
			{
				n2 = pop_numstack(ctx, err);
				if (n2 == DBL_MIN)
					return false;

				push_numstack(ctx, pop->eval(n2, n1));
			}
		}
	}
	push_opstack(ctx, op);
	return true;
}

struct eval_ctx new_ctx()
{
	struct eval_ctx ctx;
	ctx.nopstack = 0;
	ctx.nnumstack = 0;
	memset(ctx.err, 0, ERR_MSG_LEN * (sizeof ctx.err[0]));
	return ctx;
}

DOUBLE evaluate(const char* expr_to_eval, char* err)
{
	const char *expr;
	const char *tstart = NULL;
	struct op_s startop =
	{ 'X', 0, ASSOC_NONE, 0, NULL }; /* Dummy operator to mark start */
	struct op_s *op = NULL;
	DOUBLE n1, n2;
	struct op_s *lastop = &startop;
	int pos = 0;

	/* Evaluation context keeps all evaluation data encapsulated per a call stack
	 * Global/Shared variables should be only read but not rewritten.
	 */
	struct eval_ctx ctx = new_ctx();
	memset(err, 0, ERR_MSG_LEN * (sizeof err[0]));

	for (expr = expr_to_eval; *expr; ++expr, ++pos)
	{
		if (!tstart)
		{
			if ((op = getop(*expr)))
			{
				if (lastop && (lastop == &startop || lastop->op != ')'))
				{
					if (op->op == '-')
						op = getop('_');
					else if (op->op != '(')
					{
						sprintf(err,
								"Illegal use of binary operator (%c) at %d position",
								op->op, pos);
						return DBL_MIN;
					}
				}

				if (!shunt_op(&ctx, op, err))
					return DBL_MIN;

				lastop = op;
			}
			else if (isdigit(*expr))
				tstart = expr;
			else if (!isspace(*expr))
			{
				sprintf(err, "Syntax error at %d position", pos);
				return DBL_MIN;
			}
		}
		else
		{
			if (isspace(*expr))
			{
				push_numstack(&ctx, atof(tstart));
				tstart = NULL;
				lastop = NULL;
			}
			else if ((op = getop(*expr)))
			{
				push_numstack(&ctx, atof(tstart));
				tstart = NULL;
				if (!shunt_op(&ctx, op, err))
					return DBL_MIN;

				lastop = op;
			}
			else if (!isdigit(*expr) && !issep(*expr))
			{
				sprintf(err, "Syntax error at %d position", pos);
				return DBL_MIN;
			}
		}
	}
	if (tstart)
		push_numstack(&ctx, atof(tstart));

	while (ctx.nopstack)
	{
		op = pop_opstack(&ctx);
		n1 = pop_numstack(&ctx, err);
		if (n1 == DBL_MIN)
			return DBL_MIN;

		if (op->unary)
			push_numstack(&ctx, op->eval(n1, 0));
		else
		{
			n2 = pop_numstack(&ctx, err);
			if (n2 == DBL_MIN)
				return DBL_MIN;

			push_numstack(&ctx, op->eval(n2, n1));
		}
	}
	if (ctx.nnumstack != 1)
	{
		sprintf(err,
				"Number stack has %d elements after evaluation. Should be 1.",
				ctx.nnumstack);
		return DBL_MIN;
	}

	return ctx.numstack[0];
}

