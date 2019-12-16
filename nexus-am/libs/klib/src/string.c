#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t ans=0;
  for(const char* p=s;*p;p++) ans++;
  return ans;
}

char *strcpy(char* dst,const char* src) {
  if(src==NULL) return dst=NULL;
  for(size_t i=0;;i++){
    dst[i]=src[i];
    if(dst[i]=='\0') break;
  }
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  if(src==NULL) return dst=NULL;
  for(size_t i=0;i<n;i++){
    dst[i]=src[i];
    if(dst[i]=='\0') break;
  }
  return dst;
}

char* strcat(char* dst, const char* src) {
  size_t dstlen=strlen(dst);
  for(size_t i=0;;i++){
    dst[dstlen+i]=src[i];
    if(src[i]=='\0') break;
  }
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  while(*s1&&*s2){
    if(*s1>*s2) return 1;
    else if(*s1<*s2) return -1;
    s1++;
    s2++;
  }
  if(*s1) return 1;
  else if(*s2) return -1;
  else return 0; 
}

int strncmp(const char* s1, const char* s2, size_t n) {
  while(*s1&&*s2&&n){
    if(*s1>*s2) return 1;
    else if(*s1<*s2) return -1;
    s1++;
    s2++;
    n--;
  }
  if(n==0) return 0;
  else if(*s1) return 1;
  else if(*s2) return -1;
  else return 0;
}

void* memset(void* v,int c,size_t n) {
  for(size_t i=0;i<n;i++)
    ((char*)v)[i]=c;
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  char* p = (char*)in;
  char* q = (char*)out;
  for(size_t i=0;i<n;i+=4){    
    q[i]=p[i];
    if((uint32_t)in == 0x81cb044f){
      printf("n is %u  %x %x\n",n,((uint32_t*)in)[i],((uint32_t*)out)[i]);
    }
  }
    // ((char*)out)[i]=((char*)in)[i];
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  char* p=(char*)s1;
  char* q=(char*)s2;
  for(size_t i=0;i<n;i++){
    if(p[i]>q[i]) return 1;
    else if(p[i]<q[i]) return -1;
  }
  return 0;
}

#endif
