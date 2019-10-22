#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  //TODO();
  rtl_push(&id_dest->val);

  print_asm_template1(push);
}

make_EHelper(pop) {
  //TODO();
  rtl_pop(&id_dest->val);
  operand_write(id_dest,&id_dest->val);

  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  //TODO();
  // ebp/bp: 0b101 
  // esp/sp: 0b100
  int width = ( decinfo.isa.is_operand_size_16 ? 2 :4 );
  // (e)sp <- (e)bp
  rtl_lr(&s0,0b101,width);rtl_sr(0b100,&s0,width);
  // pop -> (e)bp
  rtl_pop(&s0);rtl_sr(0b101,&s0,width);

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decinfo.isa.is_operand_size_16) {
    //TODO();
	rtl_lr(&s0,R_AX,2);
	s1=( s0>>15 ? 0xffff : 0);
	rtl_sr(R_DX,&s1,2);
  }
  else {
    //TODO();
	rtl_lr(&s0,R_EAX,4);
	s1=( s0>>31 ? 0xffffffff : 0 );
	rtl_sr(R_EDX,&s1,4);
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decinfo.isa.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(&s0, &id_src->val, id_src->width);
  operand_write(id_dest, &s0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
