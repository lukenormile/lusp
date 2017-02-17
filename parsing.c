#include <stdio.h>
#include <stdlib.h>

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
#include <editline/history.h>
#endif /* _WIN32 */

int main(int argc, char **argv) {
	/* Create some parsers. */
	mpc_parser_t* Doge_Adj    = mpc_new("doge_adj");
	mpc_parser_t* Doge_Noun   = mpc_new("doge_noun");
	mpc_parser_t* Doge_Phrase = mpc_new("doge_phrase");
	mpc_parser_t* Doge        = mpc_new("doge");
	mpc_parser_t* Int 	  = mpc_new("int");
	mpc_parser_t* Float 	  = mpc_new("float");
	mpc_parser_t* Number      = mpc_new("number");
	mpc_parser_t* Operator	  = mpc_new("operator");
	mpc_parser_t* Expr	  = mpc_new("expr");
	mpc_parser_t* Lusp	  = mpc_new("lusp");

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
		operator    : '+' | '-' | '*' | '/' | '%' | \"add\" | \"sub\" |\
			      \"mul\" | \"div\" | \"mod\" ;                    \
		expr        : <number> | '(' <operator> <expr>+ ')' ;          \
		lusp        : /^/ (<operator> <expr>+ | <doge>) /$/ ;          \
		",
		Doge_Adj, Doge_Noun, Doge_Phrase, Doge,
		Float, Int, Number, Operator, Expr, Lusp
	);

	/* Print version and exit information. */
	puts("lusp Version 0.0.0.0.1");
	puts("Press Ctrl+c to Exit\n");

	/* In an infinite loop... */
	while(1) {
		/* Output our prompt and get input. */
		char *input = readline("lusp> ");

		/* Add input to history. */
		add_history(input);

		/* Attempt to parse the user input. */
		mpc_result_t r;
		if(mpc_parse("<stdin>", input, Lusp, &r)) {
			/* On success print the AST. */
			mpc_ast_print(r.output);
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
