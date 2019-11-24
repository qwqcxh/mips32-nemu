#include "cpu/exec.h"

make_EHelper(lui);

make_EHelper(ld);
make_EHelper(st);

make_EHelper(inv);
make_EHelper(nemu_trap);


/*************work******************/
make_EHelper(or);
make_EHelper(addiu);
make_EHelper(jal);
make_EHelper(jr);