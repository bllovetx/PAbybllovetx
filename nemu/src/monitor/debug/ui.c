#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}


static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_si(char *args);
static int cmd_help(char *args);
static int cmd_info(char *args);
static int cmd_x(char *args);
static int cmd_p(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si","Step N instruction exactly",cmd_si },
  { "info","info r	print register\n",cmd_info },
  { "x","scan memory",cmd_x},
  { "p","print memory",cmd_p}
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args){
	/* get number of steps N*/
	
	char *arg =strtok(NULL," ");
	int N;

	/* transform str into integer */
	if (arg == NULL) {
		/* default N=1 */
		N=1;
	}
	else {
		sscanf(arg,"%d",&N);
	}
	
	/* check N */
	//if (N==5) {return -1;}
	if (N==0) printf("Invalid command\n");
	cpu_exec(N);
	return 0;
}

/* related to isa */
void isa_reg_display();

static int cmd_info(char *args){
	/* extract first argument */
	char *arg=strtok(NULL," ");
	/* case r: info all regs */
	if( strcmp(arg,"r")==0 )  isa_reg_display();
	else printf("Unknown command\n");
	return 0;
}

/* scan memory */
static int cmd_x(char *args){
	unsigned int n,addr;
	sscanf(args,"%u 0x%x",&n,&addr);
	unsigned int num=0;
	printf("0x%x:	",addr);
	while( num<n ){
		assert( 0x100000<=addr && addr<=0x80100000 );
		if( (num%4==0)&&(num!=0) ) printf("\n0x%x:	",addr);
		printf("0x%08x	",vaddr_read(addr,4));
		num++;
		addr+=4;
	}
	printf("\n");
	return 0;
}

/* print memory--not finished */
static int cmd_p(char *args){
    bool success=true;//need initailize
	uint32_t expr_result=expr(args,&success);
	if(success){
		unsigned int n=1,addr=expr_result;
		//printf("%#x\n",addr);
		unsigned int num=0;
		printf("0x%x:	",addr);
		while( num<n ){
			assert( 0x100000<=addr && addr<=0x80100000 );
			if( (num%4==0)&&(num!=0) ) printf("\n0x%x:	",addr);
			printf("0x%80x	",vaddr_read(addr,4));
			num++;
			addr+=4;
	  	}
		printf("\n");
	}  
	return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
