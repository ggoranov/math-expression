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
