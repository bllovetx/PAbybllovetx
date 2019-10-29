#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

/* return num of char (\0 included) */

/* max printf size */
#define P_BUF_SIZE 256
char p_buf[P_BUF_SIZE];

int printf(const char *fmt, ...) {
  va_list ap;va_start(ap,fmt);
  unsigned int i;
  i=vsprintf(p_buf,fmt,ap);
  va_end(ap);
  for(char* s=p_buf;*s;s++)_putc(*s);
  return i;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int len=0;
  while(*fmt!='\0'){
	  /* no % */
	  if(*fmt!='%'){
		  *out=*fmt;
		  out++;fmt++;len++;continue;
	  }
	  /* if % fmt++ & get type */
	  fmt++;
	  switch(*fmt){
		  case 's':{
			  char* sp=va_arg(ap,char*);
			  while(*sp!='\0'){*out=*sp;out++;sp++;len++;}
			  break;}
		  case 'd':{
			  int num=va_arg(ap,int);
			  if(num==0){*out='0';out++;len++;break;}
			  if(num<0){*out='-';out++;len++;num=-num;}
			  assert(num>0);
			  char numb[12];int i=0;
			  while(num!=0){numb[i]='0'+num%10;i++;num/=10;}
			  while(i>0){i--;*out=numb[i];out++;len++;}
			  break;}
	  }
	  fmt++;
  }
  *out='\0';len++;
  return len;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;va_start(ap,fmt);
  unsigned int i;
  i=vsprintf(out,fmt,ap);
  va_end(ap);
  return i;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  assert(0);
  return 0;
}

#endif
