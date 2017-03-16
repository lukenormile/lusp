/* This file was generated from generate_ops.py */
#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "parsing.h"

lval lval_add(lval x, lval y);
lval lval_sub(lval x, lval y);
lval lval_mul(lval x, lval y);
lval lval_div(lval x, lval y);

lval lval_pow(lval x, lval y);
lval lval_min(lval x, lval y);
lval lval_max(lval x, lval y);

lval lval_neg(lval x);

lval lval_mod(lval x, lval y);

#endif /* #ifndef OPERATIONS_H */
