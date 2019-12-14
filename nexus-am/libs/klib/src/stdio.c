#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}


#define is_digit(x) ((x)>='0' && (x) <= '9')
static char* number(char* str,unsigned long num,int base,int flag){
  if(base == 16){
    *str++ = '0';
    *str++ = 'x';
  }
  if(num==0) { //special case
    *str++='0';
    return str;
  }
  if(flag==0&&(int)num < 0) {
    *str++ = '-';
    num = -(int)num;
  }
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
    if (*fmt != '%'){ //regular character
      *str++ = *fmt;
      continue;
    }
    fmt++;//skip '%'
    /* Default base */
    int base = 10;
    unsigned long num=-1;
    int flag=0;//0->signed 1->unsigned
    switch (*fmt){
      case 's':{
        int len;
        char * s = va_arg(ap, char *);
        if (!s) s = "<NULL>";
        len = strlen(s);
        for (int i = 0; i < len; ++i) *str++ = *s++;
        continue;
      }
      case 'd':
        num=(unsigned long)va_arg(ap,int);
        break;
      case 'x':
        num=(unsigned long)va_arg(ap,unsigned);
        base = 16;
        flag = 1;
        break;
    }
    str = number(str, num, base ,flag);
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

int printf(const char *fmt, ...) {
  static char buf[100];
  va_list args;
  int n;
  va_start(args,fmt);
  n=vsprintf(buf,fmt,args);
  va_end(args);
  for(int i=0;buf[i]!='\0';i++)
    _putc(buf[i]);
  return n;
}

#endif
