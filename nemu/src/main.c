#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  /* Test regex */
  FILE *fp = fopen("~/ics2019/nemu/tools/gen-expr/input","r");
  assert(fp!=NULL);
  for(int i=0;i<100;i++){
	  unsigned result;
	  char code_input[65536];
	  fscanf(fp,"%u %s\n",&result,code_input);
	  bool success=true;
	  uint32_t expr_result=expr(code_input,&success);
	  assert(result==expr_result);
  }


  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
