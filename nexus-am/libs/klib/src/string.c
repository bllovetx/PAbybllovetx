#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  assert(s!=NULL);
  size_t i=0;
  while(s[i]!='\0')i++;
  return i;
}

char *strcpy(char* dst,const char* src) {
  // Warning: can not used to creat another dst
  assert(dst!=NULL&&src!=NULL);
  size_t i=0;
  while(src[i]!='\0'){
	  dst[i]=src[i];i++;
  }
  dst[i]='\0';
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  assert(0);
  return NULL;
}

char* strcat(char* dst, const char* src) {
  // destination and source shall not overlap.
  // destination:
  //     Pointer to the destination array, which should contain a C string, and be large enough to contain the concatenated resulting string.
  assert(dst!=NULL&&src!=NULL);
  size_t i=0,j=0;
  while(dst[i]!='\0')i++;
  while(src[j]!='\0'){
	  dst[i]=src[j];
	  i++;j++;
  }
  dst[i]='\0';
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  size_t i=0;
  while( (s1[i]==s2[i]) && (s1[i]!='\0') )i++;
  int res=s1[i]-s2[i];
  if(res>0)return 1;
  if(res<0)return -1;
  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  assert(0);
  return 0;
}

void* memset(void* v,int c,size_t n) {
  // Warning: n can be larger than len,but it may cause segmental fault!
  assert(v!=NULL);
  char* pv=(char*)v;
  size_t len=strlen(pv);
  size_t i=0;
  while(i<n){
	  pv[i]=(unsigned char)c;
	  i++;
  }
  if(n>len)pv[i]='\0';
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  assert(0);
  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n){
  size_t i=0;
  if(n==0)return 0;
  const char* ps1=s1;const char* ps2=s2;
  while(i<n-1&&ps1[i]==ps2[i])i++;
  int res=(ps1[i]-ps2[i]);
  if(res>0)return 1;
  if(res<0)return -1;
  return 0;
}

#endif
