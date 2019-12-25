#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#define SYS_yield 1
extern int _syscall_(int, uintptr_t, uintptr_t, uintptr_t);

int main() {
  // return _syscall_(SYS_yield, 0, 0, 0);
  char* p=(char*)malloc(20*sizeof(char));
  p[0]='a';p[1]='b';p[2]='c';p[3]='\0';
  printf("addr of p: %x and val %s\n",p,p);
  free(p);
}
