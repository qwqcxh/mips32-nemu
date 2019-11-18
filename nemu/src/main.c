#include <stdio.h>
#include "nemu.h"

int init_monitor(int, char *[]);
void ui_mainloop(int);
uint32_t expr(char *e, bool *success);

char buf[65536];//used to save the expression

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  /***********work***************/
  #define NONE "\033[m"
  #define RED "\033[0;32;31m"
  #define LIGHT_RED "\033[1;31m"
  #define GREEN "\033[0;32;32m"
  #define LIGHT_GREEN "\033[1;32m"
  #define BLUE "\033[0;32;34m"
  #define LIGHT_BLUE "\033[1;34m"
  #define DARY_GRAY "\033[1;30m"
  #define CYAN "\033[0;36m"
  #define LIGHT_CYAN "\033[1;36m"
  #define PURPLE "\033[0;35m"
  #define LIGHT_PURPLE "\033[1;35m"
  #define BROWN "\033[0;33m"
  #define YELLOW "\033[1;33m"
  #define LIGHT_GRAY "\033[0;37m"
  #define WHITE "\033[1;37m"
  FILE* fp=fopen("tools/gen-expr/input","r");
  Assert(fp!=NULL,"open input failed!\n");
  unsigned res;
  int line=1;
  while(fscanf(fp,"%u",&res)==1){
    if(fgets(buf,65536,fp)==NULL)
      panic("input file format error!\n");
    int len=strlen(buf);
    Assert(buf[len-1]=='\n',"fgets doesn't save '\'n character");
    buf[strlen(buf)-1]='\0';
    bool valid=true;
    unsigned test_val=expr(buf,&valid);
    Assert(valid,"expression is invalid!");
    Assert(res==test_val,"test "YELLOW"%d:"NONE RED"failed: expected %u but get %u\n"NONE,line,res,test_val);
    printf("test %d:"GREEN"passed!!!\n" NONE,line);
    line++;
  }
  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
