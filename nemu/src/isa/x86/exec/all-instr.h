#include "cpu/exec.h"

/* instr-control */
make_EHelper(call);
make_EHelper(ret);

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

/* instr-logic */
make_EHelper(xor);
make_EHelper(and);
make_EHelper(setcc);

/* instr-special */
make_EHelper(inv);
make_EHelper(nemu_trap);



