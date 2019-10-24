#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  bool testflag=1;
  for(int i=0;i<8;i++){
	  //printf("%x	",ref_r->gpr[i]._32);
	  if(ref_r->gpr[i]._32!=cpu.gpr[i]._32)testflag=0;
  }
  if(ref_r->pc!=cpu.pc)testflag=0;
  //if(ref_r->EFLAGS!=cpu.EFLAGS){printf("ref:%x	dut:%x\n",ref_r->EFLAGS,cpu.EFLAGS);}//testflag=0;
  return testflag;
}

void isa_difftest_attach(void) {
}
