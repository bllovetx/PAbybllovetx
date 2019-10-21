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

/* instr-datamove */
make_EHelper(mov);
make_EHelper(push);
make_EHelper(lea);
make_EHelper(movzx);
make_EHelper(leave);

/* instr-logic */
make_EHelper(xor);
make_EHelper(and);
make_EHelper(setcc);
make_EHelper(test);

/* instr-special */
make_EHelper(inv);
make_EHelper(nemu_trap);



