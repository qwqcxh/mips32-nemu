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
    printf("buf is :%s:\n",buf);//debug
    bool valid=true;
    unsigned test_val=expr(buf,&valid);
    Assert(valid,"expression is invalid!");
    Assert(res==test_val,"expr function test failed at line:%d ,expected val %u but get %u\n",line,res,test_val);
    line++;
  }
  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
