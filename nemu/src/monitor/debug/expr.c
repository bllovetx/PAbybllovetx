#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
uint32_t isa_reg_str2val(const char *s,bool *success);
uint32_t eval(int p,int q);
	
enum {
  TK_NOTYPE = 256, TK_EQ,TK_NEQ,TK_AND,TK_OR,TK_HEX,TK_DEX,DEREF,TK_REG

  /* TODO: Add more token types */

};

/* privilege table */
#define p_token(pos) privilege(tokens[pos].type)
#define p_t(type) privilege(type)+1
int privilege(int type){
	switch(type){
		case DEREF:return 1;
		case '*':case '/':return p_t(DEREF);
		case '+':case '-':return p_t('*');
		case TK_EQ:case TK_NEQ:return p_t('+');
		case TK_AND:case TK_OR:return p_t(TK_EQ);
		default:return 0;
	}
}	

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
  {"!=", TK_NEQ},		//not equal
  {"&&", TK_AND},		//and
  {"\\|\\|", TK_OR},		//or
  {"\\*", '*'},         //multiply
  {"-", '-'},           //sub
  {"/", '/'},			//div
  {"\\(", '('},			//bra
  {"\\)", ')'},			//ket
  {"\\$[a-z,A-Z]{0,3}",TK_REG},	//register
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
			case TK_EQ:tokens[nr_token].type=TK_EQ;nr_token++;break;
			case TK_NEQ:tokens[nr_token].type=TK_NEQ;nr_token++;break;
			case TK_AND:tokens[nr_token].type=TK_AND;nr_token++;break;
			case TK_OR:tokens[nr_token].type=TK_OR;nr_token++;break;
			case TK_HEX:tokens[nr_token].type=TK_HEX;
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						tokens[nr_token].str[substr_len]='\0';nr_token++;break;
			case TK_DEX:tokens[nr_token].type=TK_DEX;
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						tokens[nr_token].str[substr_len]='\0';nr_token++;break;
			case TK_REG:tokens[nr_token].type=TK_REG;
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
		bool success=true;
		switch( tokens[p].type ){
			case TK_HEX:sscanf(tokens[p].str,"%x",&str_scan);break;
			case TK_DEX:sscanf(tokens[p].str,"%d",&str_scan);break;
			case TK_REG:str_scan=isa_reg_str2val(tokens[p].str,&success);
						if( !success ){printf("Unvalid Register name\n");assert(0);}break;
			default:assert(0);
		} 
		return str_scan;
	}
	else if( check_parentheses(p,q)==true ){
		return eval(p+1,q-1);
	}
	else if( tokens[p].type==DEREF ){
		return vaddr_read(eval(p+1,q),4);
	}
	else{ 
		int fd_main=-1,m_op=-1;
		for(int i=p;i<=q;i++){
			switch( tokens[i].type ){
				case '(':fd_main++;break;
				case ')':fd_main--;break;
				default :if( fd_main<0 && (m_op==-1||p_token(i)>=p_token(m_op)) ){
							 m_op=i;
						 }
						 break;
			}
		}
		assert(p<m_op&&m_op<q);
		assert(privilege(tokens[m_op].type)>1);
		assert(m_op!=-1);
		uint32_t left_main=eval(p,m_op-1),right_main=eval(m_op+1,q);
		//printf("%d	%d\n",left_main,right_main);
		switch( tokens[m_op].type ){
			case TK_AND:return left_main&&right_main;break;
			case TK_OR:return left_main||right_main;break;
			case TK_EQ:return left_main==right_main;break;
			case TK_NEQ:return left_main!=right_main;break;
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
  for( int i=0;i<nr_token;i++ ){
	 if( tokens[i].type=='*' && (i==0||p_token(i-1)>0) ){tokens[i].type=DEREF;}
  }
  //printf("%d	%d\n",privilege(TK_NOTYPE),privilege(TK_EQ));
  /* TODO: Insert codes to evaluate the expression. */
  return eval(0,nr_token-1); 

}
