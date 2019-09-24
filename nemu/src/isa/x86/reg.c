#include "nemu.h"
#include <stdlib.h>
#include <time.h>

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
  srand(time(0));
  uint32_t sample[8];
  uint32_t pc_sample = rand();
  cpu.pc = pc_sample;

  int i;
  for (i = R_EAX; i <= R_EDI; i ++) {
    sample[i] = rand();
    reg_l(i) = sample[i];
    assert(reg_w(i) == (sample[i] & 0xffff));
  }

  assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
  assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
  assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
  assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
  assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
  assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
  assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
  assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

  assert(sample[R_EAX] == cpu.eax);
  assert(sample[R_ECX] == cpu.ecx);
  assert(sample[R_EDX] == cpu.edx);
  assert(sample[R_EBX] == cpu.ebx);
  assert(sample[R_ESP] == cpu.esp);
  assert(sample[R_EBP] == cpu.ebp);
  assert(sample[R_ESI] == cpu.esi);
  assert(sample[R_EDI] == cpu.edi);

  assert(pc_sample == cpu.pc);
}

void isa_reg_display() {
	int index=0,width=1;
	while( width<=4 ){
		switch( width ){
			case 1:printf("%s:        %02xH\t",reg_name(index,width),reg_b(index));break;
			case 2:printf("%s:      %04xH\t",reg_name(index,width),reg_w(index));break;
			case 4:printf("%s: %08xH\t",reg_name(index,width),reg_l(index));break;
		}
		index++;
		if( !(index&0x3) ) printf("\n");
		if( index/8 ) width*=2;
		index%=8;
	}
}


uint32_t isa_reg_str2val(const char *s, bool *success) {
  //printf("%s\n",s);
  assert(*s=='$');
  /* lowercase 32_16_8 */
  for( int i=0;i<8;i++ ){if(strcmp(s+1,regsl[i])==0)return reg_l(i);}
  for( int i=0;i<8;i++ ){if(strcmp(s+1,regsw[i])==0)return reg_w(i);}
  for( int i=0;i<8;i++ ){if(strcmp(s+1,regsb[i])==0)return reg_b(i);}
  /* highercase 32_16_8 */
  const int trans_char='A'-'a';
  char s_cap[4];
  strcpy(s_cap,s+1);
  int cap_i=0;
  while(*(s_cap+cap_i)!='\0'){
	  *(s_cap+cap_i)-=trans_char;
	  cap_i++;
  }
  printf("%s\n",s_cap);
  assert(cap_i<=3);

  return 0x100000;
}
