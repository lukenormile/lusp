#ifndef PARSING_H
#define PARSING_H

#include "mpc.h"

/* Functions needed in Windows only. */
#ifdef _WIN32
char *readline(char *prompt);
void add_history(char *unused);
#endif /* _WIN32 */

/* Enumerate the possible lval types. */
enum { LVAL_INT, LVAL_FLOAT, LVAL_ERR, LVAL_DOGE };

/* Enumerate the possible error types. */
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM, LERR_CANT_EVAL };

/* Make one type for either number type. */
typedef union {
	long integer;
	float floating;
} number;

/* The union lvalue holds the actual value. This union is held in the
   struct, which also holds the type descriptor. */
typedef union {
	number num;
	char *doge;
	int err;
} lvalue;
typedef struct{
	lvalue val;
	int type;
} lval;

lval lval_int(long x);

lval lval_float(float x);

lval lval_err(int x);

lval lval_doge();

lval eval_op(lval x, char *op, lval y);

lval eval_tinyop(lval x, char *op);

lval eval_expr(mpc_ast_t *t);

lval eval_doge(void);

lval eval(mpc_ast_t *t);

int main(void);

#endif /* PARSING_H */
