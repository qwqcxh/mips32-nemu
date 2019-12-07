#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  return 0;
}

// int vsprintf(char *out, const char *fmt, va_list ap) {
//   return 0;
// }

// int sprintf(char *out, const char *fmt, ...) {
//   return 0;
// }

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}


#define is_digit(x) ((x)>='0' && (x) <= '9')
static char* number(char* str,unsigned long num,int base){
  char tmp[100];
  int i=0;
  while(num){
    tmp[i]=num%base;
    num/=base;
    if(tmp[i]<10) tmp[i]+='0';
    else tmp[i]=tmp[i]-10+'a';
    i++;
  }
  while(--i>=0){
    *str++=tmp[i];
  }
  return str;
}

int vsprintf(char *out, const char *fmt, va_list ap){
  char *str;
  for (str = out; *fmt; fmt++){
    int base = 10; //default base if 10
    if (*fmt != '%'){ //regular character
      *str++ = *fmt;
      continue;
    }
    fmt++;//skip '%'
    /* Default base */
    base = 10;
    unsigned long num=-1;
    switch (*fmt){
      case 's':{
        int len;
        char * s = va_arg(ap, char *);
        if (!s) s = "<NULL>";
        len = strlen(s);
        for (int i = 0; i < len; ++i) *str++ = *s++;
        continue;
      }
      case 'd':{
        num=(unsigned long)va_arg(ap,int);
      }
    }
    str = number(str, num, base);
  }
  *str = '\0';
  return str - out;
}

int sprintf(char *out, const char *fmt, ...)
{
    va_list args;
    int n;

    va_start(args, fmt);
    n = vsprintf(out, fmt, args);
    va_end(args);

    return n;
}

#endif
