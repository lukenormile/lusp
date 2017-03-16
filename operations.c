/* This file was generated from generate_ops.py */

#include "operations.h"
#include "parsing.h"

lval lval_add(lval x, lval y) {
	if(x.type == LVAL_INT && y.type == LVAL_INT) {
		return lval_int(x.val.num.integer + y.val.num.integer);
	}
	if(x.type == LVAL_INT && y.type == LVAL_FLOAT) {
		return lval_float(x.val.num.integer + y.val.num.floating);
	}
	if(x.type == LVAL_FLOAT && y.type == LVAL_INT) {
		return lval_float(x.val.num.floating + y.val.num.integer);
	}
	if(x.type == LVAL_FLOAT && y.type == LVAL_FLOAT) {
		return lval_float(x.val.num.floating + y.val.num.floating);
	}
	return lval_err(LERR_BAD_NUM);
}

lval lval_sub(lval x, lval y) {
	if(x.type == LVAL_INT && y.type == LVAL_INT) {
		return lval_int(x.val.num.integer - y.val.num.integer);
	}
	if(x.type == LVAL_INT && y.type == LVAL_FLOAT) {
		return lval_float(x.val.num.integer - y.val.num.floating);
	}
	if(x.type == LVAL_FLOAT && y.type == LVAL_INT) {
		return lval_float(x.val.num.floating - y.val.num.integer);
	}
	if(x.type == LVAL_FLOAT && y.type == LVAL_FLOAT) {
		return lval_float(x.val.num.floating - y.val.num.floating);
	}
	return lval_err(LERR_BAD_NUM);
}

lval lval_mul(lval x, lval y) {
	if(x.type == LVAL_INT && y.type == LVAL_INT) {
		return lval_int(x.val.num.integer * y.val.num.integer);
	}
	if(x.type == LVAL_INT && y.type == LVAL_FLOAT) {
		return lval_float(x.val.num.integer * y.val.num.floating);
	}
	if(x.type == LVAL_FLOAT && y.type == LVAL_INT) {
		return lval_float(x.val.num.floating * y.val.num.integer);
	}
	if(x.type == LVAL_FLOAT && y.type == LVAL_FLOAT) {
		return lval_float(x.val.num.floating * y.val.num.floating);
	}
	return lval_err(LERR_BAD_NUM);
}

lval lval_div(lval x, lval y) {
	if(x.type == LVAL_INT && y.type == LVAL_INT) {
		return lval_int(x.val.num.integer / y.val.num.integer);
	}
	if(x.type == LVAL_INT && y.type == LVAL_FLOAT) {
		return lval_float(x.val.num.integer / y.val.num.floating);
	}
	if(x.type == LVAL_FLOAT && y.type == LVAL_INT) {
		return lval_float(x.val.num.floating / y.val.num.integer);
	}
	if(x.type == LVAL_FLOAT && y.type == LVAL_FLOAT) {
		return lval_float(x.val.num.floating / y.val.num.floating);
	}
	return lval_err(LERR_BAD_NUM);
}

/* Other operations */
lval lval_pow(lval x, lval y) {
	if(x.type == LVAL_INT && y.type == LVAL_INT) {
		return lval_int(powl(x.val.num.integer,
			 y.val.num.integer));
	}
	if(x.type == LVAL_INT && y.type == LVAL_FLOAT) {
		return lval_float(powf(x.val.num.integer,
			 y.val.num.floating));
	}
	if(x.type == LVAL_FLOAT && y.type == LVAL_INT) {
		return lval_float(powf(x.val.num.floating,
			 y.val.num.integer));
	}
	if(x.type == LVAL_FLOAT && y.type == LVAL_FLOAT) {
		return lval_float(powf(x.val.num.floating,
			 y.val.num.floating));
	}
	return lval_err(LERR_BAD_NUM);
}

lval lval_min(lval x, lval y) {
	if(x.type == LVAL_INT && y.type == LVAL_INT) {
		return lval_int(fminl(x.val.num.integer,
			 y.val.num.integer));
	}
	if(x.type == LVAL_INT && y.type == LVAL_FLOAT) {
		return lval_float(fminf(x.val.num.integer,
			 y.val.num.floating));
	}
	if(x.type == LVAL_FLOAT && y.type == LVAL_INT) {
		return lval_float(fminf(x.val.num.floating,
			 y.val.num.integer));
	}
	if(x.type == LVAL_FLOAT && y.type == LVAL_FLOAT) {
		return lval_float(fminf(x.val.num.floating,
			 y.val.num.floating));
	}
	return lval_err(LERR_BAD_NUM);
}

lval lval_max(lval x, lval y) {
	if(x.type == LVAL_INT && y.type == LVAL_INT) {
		return lval_int(fmaxl(x.val.num.integer,
			 y.val.num.integer));
	}
	if(x.type == LVAL_INT && y.type == LVAL_FLOAT) {
		return lval_float(fmaxf(x.val.num.integer,
			 y.val.num.floating));
	}
	if(x.type == LVAL_FLOAT && y.type == LVAL_INT) {
		return lval_float(fmaxf(x.val.num.floating,
			 y.val.num.integer));
	}
	if(x.type == LVAL_FLOAT && y.type == LVAL_FLOAT) {
		return lval_float(fmaxf(x.val.num.floating,
			 y.val.num.floating));
	}
	return lval_err(LERR_BAD_NUM);
}

/* Single-operand operations */
lval lval_neg(lval x) {
	if(x.type == LVAL_INT) { return lval_int(-x.val.num.integer); }
	if(x.type == LVAL_FLOAT) { return lval_float(-x.val.num.floating); }
	return lval_err(LERR_BAD_NUM);
}

/* Integer-only operations */
lval lval_mod(lval x, lval y) {
	return lval_int(x.val.num.integer % y.val.num.integer);
}
