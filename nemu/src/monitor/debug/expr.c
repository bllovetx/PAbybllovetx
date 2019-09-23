#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

uint32_t eval(int p,int q);
	

enum {
  TK_NOTYPE = 256, TK_EQ,TK_HEX,TK_DEX

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},         // equal
  {"\\*", '*'},         //multiply
  {"-", '-'},           //sub
  {"/", '/'},			//div
  {"\\(", '('},			//bra
  {"\\)", ')'},			//ket
  {"0x[0-9,a-f,A-F]+",TK_HEX},	//hex
  {"[0-9]+",TK_DEX}		//dex
  
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
			case '+':tokens[nr_token].type='+';nr_token++;break;
			case '-':tokens[nr_token].type='-';nr_token++;break;
			case '*':tokens[nr_token].type='*';nr_token++;break;
			case '/':tokens[nr_token].type='/';nr_token++;break;
			case '(':tokens[nr_token].type='(';nr_token++;break;
			case ')':tokens[nr_token].type=')';nr_token++;break;
			case TK_HEX:tokens[nr_token].type=TK_HEX;
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						tokens[nr_token].str[substr_len]='\0';nr_token++;break;
			case TK_DEX:tokens[nr_token].type=TK_DEX;
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						tokens[nr_token].str[substr_len]='\0';nr_token++;break;
			default: break; 
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_parentheses(int p,int q){
	int ch_p=-1;
	for(int i=p;i<q;i++){
		switch( tokens[i].type ){
			case '(':ch_p++;break;
			case ')':ch_p--;break;
			default :break;
		}
		if( ch_p<0 ) return false;
	}
	assert(tokens[q].type==')');
	return true;
}

uint32_t eval(int p,int q){
	if( p>q ){
		/* bad expr */
		printf("Unvalid Expression\n");
		assert(0);
		return 0;
	}
	else if( p==q ){
		uint32_t str_scan;
		switch( tokens[p].type ){
			case TK_HEX:sscanf(tokens[p].str,"%x",&str_scan);break;
			case TK_DEX:sscanf(tokens[p].str,"%d",&str_scan);break;
			default:assert(0);
		}
		return str_scan;
	}
	else if( check_parentheses(p,q)==true ){
		return eval(p+1,q-1);
	}
	else{
		int fd_main=-1,m_op=-1;
		for(int i=p;i<=q;i++){
			switch( tokens[i].type ){
				case '(':fd_main++;break;
				case ')':fd_main--;break;
				case '+':if(fd_main<0&&m_op<0){m_op=i;};break;
				case '-':if(fd_main<0&&m_op<0){m_op=i;};break;
				case '*':if(fd_main<0){m_op=i;};break;
				case '/':if(fd_main<0){m_op=i;};break;
				default :break;
			}
		}
		assert(p<m_op&&m_op<q);
		assert(m_op!=-1);
		uint32_t left_main=eval(p,m_op-1),right_main=eval(m_op+1,q);
		printf("%d	%d\n",left_main,right_main);
		switch( tokens[m_op].type ){
			case '+':return left_main+right_main;break;
			case '-':return left_main-right_main;break;
			case '*':return left_main*right_main;break;
			case '/':
					if( right_main==0 )printf("Unvalid Expression");
					assert(right_main!=0);
					return left_main/right_main;break;
			default :assert(0);break;
		}

	}
}


uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  return eval(0,nr_token-1); 

}
