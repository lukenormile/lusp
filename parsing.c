#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <editline/readline.h>

#include "parsing.h"
#include "mpc.h"

/* If we are compiling on Windows compile these functions. */
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/* Fake readline function for Windows: */
char *readline(char *prompt) {
	fputs(prompt, stdout);
	fgets(buffer, 2048, stdin);
	char *cpy = malloc(strlen(buffer)+1);
	strcpy(cpy, buffer);
	cpy[strlen(cpy)-1] = '\0';
	return cpy;
}

/* Fake add_history function for Windows: */
void add_history(char *unused) {}

/* If not compiling on Windows include the real editline headers. */
#else
#include <editline/readline.h>

/* editline/history.h is not needed on MacOS.
   Additional nuance may be needed here if compilation doesn't work on
   other Apple products, e.g. iOS. */
#ifdef linux
#include <editline/history.h>
#endif /* linux */
#endif /* _WIN32 */

/* Create a new integer-type lval. */
lval lval_int(long x) {
	lval v;
	v.type = LVAL_INT;
	v.val.num.integer = x;
	return v;
}

/* Create a new float-type lval. */
lval lval_float(float x) {
	lval v;
	v.type = LVAL_INT;
	v.val.num.floating = x;
	return v;
}

/* Create a new error-type lval. */
lval lval_err(int x) {
	lval v;
	v.type = LVAL_ERR;
	v.val.err = x; 
	return v;
}

/* Create a new doge-type lval. */
lval lval_doge() {
	lval v;
	v.type = LVAL_DOGE;
	v.val.doge = "Wow";
	return v;
}

/* Print an lval without a newline. */
void lval_print(lval v) {
	switch(v.type) {
		case LVAL_INT:
			printf("%li", v.val.num.integer);
			break;
		case LVAL_FLOAT:
			printf("%f", v.val.num.floating);
		case LVAL_ERR:
			switch(v.val.err) {
				case LERR_DIV_ZERO:
					printf("Error: division by zero.");
					break;
				case LERR_BAD_OP:
					printf("Error: invalid operator.");
					break;
				case LERR_BAD_NUM:
					printf("Error: invalid number.");
					break;
				case LERR_CANT_EVAL:
					printf("Error: couldn't evaluate input.");
					break;
			}
		case LVAL_DOGE:
			printf("%s", v.val.doge);
			break;
	}
}

void lval_println(lval v) {
	lval_print(v);
	putchar('\n');
}

/* Use operator string to see which operation to perform and do it. */
lval eval_op(lval x, char *op, lval y) {
	/* If either value is an error return it. */
	if(x.type == LVAL_ERR) { return x; }
	if(y.type == LVAL_ERR) { return y; }

	/* Figure out which operation we're doing. */
	if(strcmp(op, "+") == 0) { return lval_num(x.val.num + y.val.num); }
	if(strcmp(op, "-") == 0) { return lval_num(x.val.num - y.val.num); }
	if(strcmp(op, "*") == 0) { return lval_num(x.val.num * y.val.num); }
	if(strcmp(op, "/") == 0 || strcmp(op, "div") == 0) { 
		/* If second operator is zero return a Divide by Zero error. */
		return y.val.num == 0
			? lval_err(LERR_DIV_ZERO)
			: lval_num(x.val.num / y.val.num);
		/* Note to self: this is my first time using the 
		   conditional operator. I need to learn its ins
		   and outs better. */
	}
	if(strcmp(op, "%") == 0) { return lval_num(x.val.num % y.val.num); }
	if(strcmp(op, "^") == 0) { return lval_num(powl(x.val.num,y.val.num)); }
	if(strcmp(op, "add") == 0) { return lval_num(x.val.num + y.val.num); }
	if(strcmp(op, "sub") == 0) { return lval_num(x.val.num - y.val.num); }
	if(strcmp(op, "mul") == 0) { return lval_num(x.val.num * y.val.num); }
	if(strcmp(op, "mod") == 0) { return lval_num(x.val.num % y.val.num); }
	if(strcmp(op, "pow") == 0) { return lval_num(powl(x.val.num,y.val.num)); }
	if(strcmp(op, "min") == 0) { return lval_num(fminl(x.val.num,y.val.num)); }
	if(strcmp(op, "max") == 0) { return lval_num(fmaxl(x.val.num,y.val.num)); }
	return lval_err(LERR_BAD_OP);
}

/* Evaluate an operator with a single operand. */ 
lval eval_tinyop(lval x, char *op) {
	/* If x is an error return it. */
	if(x.type == LVAL_ERR) { return x; }

	if(strcmp(op, "-") == 0) { return lval_num(-(x.val.num)); }
	else { return x; }
}

/* Evaluate an expression recursively. */
lval eval_expr(mpc_ast_t *t) {
	/* If tagged as number return it directly, unless there
	   is an error in conversion. */
	if(strstr(t->tag, "float")) {
		/* Handle any errors in conversion. */
		errno = 0;
		float x = strtof(t->contents, NULL, 10);
		return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
	} else if(strstr(t->tag, "int")) {
		/* Handle any errors in conversion. */
		errno = 0;
		long x = strtol(t->contents, NULL, 10);
		return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
	/* The operator is always the second child. */
	char *op = t->children[1]->contents;
	/* We store the third child in 'x'/ */
	lval x = eval_expr(t->children[2]);
	
	/* If there are no more children, perform a single-operand operation. */
	int i = 3;
	if(!strstr(t->children[i]->tag, "expr")) {
		x = eval_tinyop(x, op);
	} else {
		do {
			/* Iterate the remaining children and combine results. */
			x = eval_op(x, op, eval_expr(t->children[i]));
			i++;
		} while(strstr(t->children[i]->tag, "expr"));
	}

	return x;
}

/* Evaluate a Doge. */
lval eval_doge() {
	return lval_doge();
}

/* Begin to evaluate an input. */
lval eval(mpc_ast_t *t) {
	/* Default to error. */
	lval result = lval_err(LERR_CANT_EVAL);

	/* Get the type of construction. */
	mpc_ast_t *construction = t->children[1];

	/* If tagged as an operator evaluate it as an expression. */
	if (strstr(construction->tag, "operator")) {
		result = eval_expr(t);
	} else if (strstr(construction->tag, "doge")) {
		result = eval_doge();
	}

	return result;
}

int main(void) {
	/* Create some parsers. */
	mpc_parser_t* Doge_Adj    = mpc_new("doge_adj");
	mpc_parser_t* Doge_Noun   = mpc_new("doge_noun");
	mpc_parser_t* Doge_Phrase = mpc_new("doge_phrase");
	mpc_parser_t* Doge        = mpc_new("doge");
	mpc_parser_t* Int   	  = mpc_new("int");
	mpc_parser_t* Float 	  = mpc_new("float");
	mpc_parser_t* Number      = mpc_new("number");
	mpc_parser_t* Operator	  = mpc_new("operator");
	mpc_parser_t* Expr  	  = mpc_new("expr");
	mpc_parser_t* Lusp  	  = mpc_new("lusp");

	/* Define the parsers with the following Language: */
	mpca_lang(MPCA_LANG_DEFAULT,
		"						               \
		doge_adj    : \"wow\" | \"many\" | \"so\" | \"such\" ;         \
		doge_noun   : \"lisp\" | \"language\" | \"book\" |             \
		              \"build\" | \"C\" ;                              \
		doge_phrase : <doge_adj> <doge_noun> ;                         \
		doge        : \"doge\" <doge_phrase>* \"endoge\" ;             \
		float       : /(0|-?[1-9][0-9]*)\\.[0-9]+/ ;                   \
		int         : /-?[0-9]+/ ;                                     \
		number      : <float> | <int> ;                                \
		operator    : '+' | '-' | '*' | '/' | '%' | '^' |              \
				      \"add\" | \"sub\" | \"mul\" | \"div\" |          \
					  \"mod\" | \"pow\" | \"min\" | \"max\" ;          \
		expr        : <number> | '(' <operator> <expr>+ ')' ;          \
		lusp        : /^/ (<operator> <expr>+ | <doge>) /$/ ;          \
		",
		Doge_Adj, Doge_Noun, Doge_Phrase, Doge,
		Float, Int, Number, Operator, Expr, Lusp
	);

	/* Print version and exit information. */
	puts("lusp Version 0.0.0.0.3");
	puts("'q' or 'quit' to exit");

	/* Loop until user quits or Ctrl-C signal is received. */
	while(1) {
		/* Output our prompt and get input. */
		char *input = readline("lusp> ");

		/* If 'quit', exit gracefully. */
		if((strncmp(input, "quit", 5) == 0) || (strncmp(input, "q", 2) == 0)) {
			free(input);
			break;
		}

		/* Add input to history. */
		add_history(input);

		/* Attempt to parse the user input. */
		mpc_result_t r;
		if(mpc_parse("<stdin>", input, Lusp, &r)) {
			/* On success print the AST. Enable for debugging. */
			// mpc_ast_print(r.output);

			/* Evaluate the input and print the result. */
			lval result = eval(r.output);
			lval_println(result);

			/* Clear out the AST. */
			mpc_ast_delete(r.output);
		} else {
			/* Otherwise print the error. */
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}

		/* Free retrieved input. */
		free(input);
	}

	/* Undefine and delete our Parsers. */
	mpc_cleanup(4, Number, Operator, Expr, Lusp);

	return 0;
}
