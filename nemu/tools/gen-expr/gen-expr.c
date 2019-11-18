#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

// this should be enough
static char buf[65536];
/********************work***********************/
#define MAX_NUMBER 60000
unsigned choose(unsigned x){//gen num randomly in range[0,x];
  return rand()%x;
}

int gen_num(int idx){//gen num randomly in range[0,0xffffffff];
  char str[20];
  unsigned int val=rand()%RAND_MAX;
  sprintf(str,"%u",val);
  strcpy(buf+idx,str); //fix a bug
  idx+=strlen(str);
  buf[idx++]=' ';
  return idx;
}

int gen_rand_space(int idx){ //gen a space randomly
  if(rand()%2){
    buf[idx]=' ';
    return idx+1;
  }
  else return idx;
}

int gen_rand_op(int idx){ //gen a operator randmly
  static char ops[]={'+','-','*','/'};
  int len=sizeof(ops)/sizeof(char);
  buf[idx]=ops[rand()%len];
  return idx+1;
}

static inline int gen_rand_expr(int idx) {
  if(idx>MAX_NUMBER) return gen_num(idx); //if idx is big enough,ending gen_rand_expr as fast as possible
  switch (rand()%3) {
    case 0: return gen_num(idx);
    case 1: 
      buf[idx++]='(';
      idx=gen_rand_space(idx);
      idx=gen_rand_expr(idx);
      idx=gen_rand_space(idx);
      buf[idx++]=')'; 
      return idx;
    default: 
      idx=gen_rand_expr(idx);
      idx=gen_rand_space(idx);
      idx=gen_rand_op(idx);
      idx=gen_rand_space(idx);
      idx=gen_rand_expr(idx);
      return idx;
  }
}
/**********************done***************************/


static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    buf[gen_rand_expr(0)]='\0';

    sprintf(code_buf, code_format, buf);
    //change the constant to unsigned
    for(int j=0;code_buf[j];j++){
      if(code_buf[j]==' '&&j&&isdigit(code_buf[j-1]))
        code_buf[j]='u';
    }

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc -Werror /tmp/.code.c -o /tmp/.expr 2>/dev/null");
    if (ret != 0) {
      i--;continue;
    }

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    assert(fscanf(fp, "%d", &result)==1);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
