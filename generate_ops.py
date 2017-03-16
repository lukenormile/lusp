main_ops = ["add", "sub", "mul", "div"]
main_symbols = ['+', '-', '*', '/']

other_ops = ["pow", "min", "max"]
other_op_fxns = [["powl", "powf"], ["fminl", "fminf"], ["fmaxl", "fmaxf"]]

tiny_ops = ["neg"]
tiny_symbols = ['-']

int_ops = ["mod"]
int_symbols = ['%']

with open('operations.c', 'w') as cfile:
    cfile.write("/* This file was generated from generate_ops.py */\n\n");
    cfile.write("#include \"operations.h\"\n")
    cfile.write("#include \"parsing.h\"\n\n")
    for op, sym in zip(main_ops, main_symbols):
        cfile.write("lval lval_%s(lval x, lval y) {\n" % op)
        cfile.write("\tif(x.type == LVAL_INT && y.type == LVAL_INT) {\n")
        cfile.write("\t\treturn lval_int(x.val.num.integer %s y.val.num.integer);\n" % sym)
        cfile.write("\tif(x.type == LVAL_INT && y.type == LVAL_FLOAT) {\n")
        cfile.write("\t\treturn lval_float(x.val.num.integer %s y.val.num.floating);\n" % sym)
        cfile.write("\tif(x.type == LVAL_FLOAT && y.type == LVAL_INT) {\n")
        cfile.write("\t\treturn lval_float(x.val.num.floating %s y.val.num.integer);\n" % sym)
        cfile.write("\tif(x.type == LVAL_FLOAT && y.type == LVAL_FLOAT) {\n")
        cfile.write("\t\treturn lval_float(x.val.num.floating %s y.val.num.floating);\n" % sym)
        cfile.write("}\n\n")

    cfile.write("/* Other operations */\n");
    for op, fxn in zip(other_ops, other_op_fxns):
        cfile.write("lval lval_%s(lval x, lval y) {\n" % op)
        cfile.write("\tif(x.type == LVAL_INT && y.type == LVAL_INT) {\n")
        cfile.write("\t\treturn lval_int(%s(x.val.num.integer,\n\t\t\t y.val.num.integer));\n" % fxn[0])
        cfile.write("\tif(x.type == LVAL_INT && y.type == LVAL_FLOAT) {\n")
        cfile.write("\t\treturn lval_float(%s(x.val.num.integer,\n\t\t\t y.val.num.floating));\n" % fxn[1])
        cfile.write("\tif(x.type == LVAL_FLOAT && y.type == LVAL_INT) {\n")
        cfile.write("\t\treturn lval_float(%s(x.val.num.floating,\n\t\t\t y.val.num.integer));\n" % fxn[1])
        cfile.write("\tif(x.type == LVAL_FLOAT && y.type == LVAL_FLOAT) {\n")
        cfile.write("\t\treturn lval_float(%s(x.val.num.floating,\n\t\t\t y.val.num.floating));\n" % fxn[1])
        cfile.write("}\n\n")

    cfile.write("/* Single-operand operations */\n");
    for op, sym in zip(tiny_ops, tiny_symbols):
        cfile.write("lval lval_%s(lval x) {\n" % op)
        cfile.write("\treturn %sx;\n" % sym)
        cfile.write("}\n")

    cfile.write("/* Integer-only operations */\n");
    for op, sym in zip(int_ops, int_symbols):
        cfile.write("lval lval_%s(lval x) {\n" % op)
        cfile.write("\treturn lval_int(x %s y);\n" % sym)
        cfile.write("}\n")

with open('operations.h', 'w') as hfile:
    hfile.write("/* This file was generated from generate_ops.py */\n");
    hfile.write("#ifndef OPERATIONS_H\n");
    hfile.write("#define OPERATIONS_H\n\n");
    hfile.write("#include \"parsing.h\"\n\n");

    for op in main_ops:
        hfile.write("lval lval_%s(lval x, lval y);\n" % op)
    hfile.write("\n");
    for op in other_ops:
        hfile.write("lval lval_%s(lval x, lval y);\n" % op)
    hfile.write("\n");
    for op in tiny_ops:
        hfile.write("lval lval_%s(lval x);\n" % op)

    hfile.write("\n#endif /* #ifndef OPERATIONS_H */\n");
