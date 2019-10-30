#include "cpu/exec.h"
#include "cc.h"

make_EHelper(test) {
  //TODO();
  rtl_and(&s0,&id_dest->val,&id_src->val);
  rtl_update_ZFSF(&s0,id_dest->width);

  print_asm_template2(test);
}

make_EHelper(and) {
  //TODO();
  rtl_and(&s0,&id_dest->val,&id_src->val);
  operand_write(id_dest,&s0);
  rtl_update_ZFSF(&s0,id_dest->width);
 
  print_asm_template2(and);
}

make_EHelper(xor) {
  //TODO();
  rtl_xor(&s0,&id_dest->val,&id_src->val);
  operand_write(id_dest,&s0);
 //printf("%x\n",cpu.eax);
  rtl_update_ZFSF(&s0,id_dest->width);

  print_asm_template2(xor);
}

make_EHelper(or) {
  //TODO();
  rtl_or(&s0,&id_dest->val,&id_src->val);
  operand_write(id_dest,&s0);
  rtl_update_ZFSF(&s0,id_dest->width);
 
  print_asm_template2(or);
}

make_EHelper(sar) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_sar(&s0,&id_dest->val,&id_src->val);
  operand_write(id_dest,&s0);
  rtl_update_ZFSF(&s0,id_dest->width);

  print_asm_template2(sar);
}

make_EHelper(shl) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shl(&s0,&id_dest->val,&id_src->val);
  operand_write(id_dest,&s0);
  rtl_update_ZFSF(&s0,id_dest->width);


  print_asm_template2(shl);
}

make_EHelper(shr) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shr(&s0,&id_dest->val,&id_src->val);
  operand_write(id_dest,&s0);
  rtl_update_ZFSF(&s0,id_dest->width);


  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decinfo.opcode & 0xf;

  rtl_setcc(&s0, cc);
  operand_write(id_dest, &s0);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  //TODO();
  rtl_not(&s0,&id_dest->val);
  operand_write(id_dest,&s0);

  print_asm_template1(not);
}

/* additional instr 
** --first occur
**   in microbench/md5
*/

/* rotate left without CF */
make_EHelper(rol) {
  if(id_dest->width!=4)assert( id_src->val < (id_dest->width*8) );
  s0=(8*id_dest->width)-id_src->val;
  rtl_shl(&s1,&id_dest->val,&id_src->val);
  rtl_shr(&s2,&id_dest->val,&s0);
  rtl_add(&s1,&s1,&s2);
  rtl_andi(&s1,&s1,  0xffffffffu >> ((4 - id_dest->width) * 8));
  operand_write(id_dest,&s1);

  print_asm_template2(rol);
}