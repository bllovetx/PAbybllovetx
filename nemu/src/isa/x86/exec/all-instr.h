#include "cpu/exec.h"

/* instr-control */
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jcc);

/* instr-prefix */
make_EHelper(operand_size);

/* instr-system */

/* instr-arith */
make_EHelper(sub);
make_EHelper(add);
make_EHelper(cmp);
make_EHelper(inc);
make_EHelper(adc);

/* instr-datamove */
make_EHelper(mov);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(lea);
make_EHelper(movzx);
make_EHelper(leave);

/* instr-logic */
make_EHelper(xor);
make_EHelper(and);
make_EHelper(setcc);
make_EHelper(test);
make_EHelper(or);

/* instr-special */
make_EHelper(inv);
make_EHelper(nemu_trap);
make_EHelper(nop);



