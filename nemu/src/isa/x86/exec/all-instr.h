#include "cpu/exec.h"

/* instr-control */
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jcc);
make_EHelper(jmp);
make_EHelper(call_rm);
make_EHelper(jmp_rm);

/* instr-prefix */
make_EHelper(operand_size);

/* instr-system */
make_EHelper(out);
make_EHelper(in);

/* instr-arith */
make_EHelper(sub);
make_EHelper(add);
make_EHelper(cmp);
make_EHelper(inc);
make_EHelper(dec);
make_EHelper(adc);
make_EHelper(sbb);
make_EHelper(mul);
make_EHelper(imul1);
make_EHelper(imul2);
make_EHelper(div);
make_EHelper(idiv);
make_EHelper(neg);

/* instr-datamove */
make_EHelper(mov);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(lea);
make_EHelper(movzx);
make_EHelper(movsx);
make_EHelper(leave);
make_EHelper(cltd);

/* instr-logic */
make_EHelper(xor);
make_EHelper(and);
make_EHelper(setcc);
make_EHelper(test);
make_EHelper(or);
make_EHelper(sar);
make_EHelper(shl);
make_EHelper(shr);
make_EHelper(not);

/* instr-special */
make_EHelper(inv);
make_EHelper(nemu_trap);
make_EHelper(nop);



