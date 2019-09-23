#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
int choose(int ch_max){return rand()%ch_max;}
int pos=0;
static char buf[65536];
static inline void gen_num(){	
		  switch( choose(2) ){
			  case 0:pos+=sprintf(buf+pos,"0x%x",choose(10)+1);
			  default:pos+=sprintf(buf+pos,"%d",choose(10)+1);
		  }
}
static inline void gen_op(){
	switch(choose(4)){
		case 0:buf[pos]='+';pos++;break;
		case 1:buf[pos]='-';pos++;break;
		case 2:buf[pos]='*';pos++;break;
		default:buf[pos]='/';pos++;break;
	}
}	
static inline void gen_rand_expr() {
  switch( choose(4) ){ 
	  case 0:gen_num();break;
	  case 1:buf[pos]=' ';pos++;gen_rand_expr();break;
	  case 2:buf[pos]='(';pos++;
			 gen_rand_expr();
			 buf[pos]=')';pos++;
			 break;
	  default:gen_rand_expr();gen_op();gen_num();break;
   }
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
	pos=0;
    gen_rand_expr();
	assert( pos<65536 );
	buf[pos]='\0';

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
