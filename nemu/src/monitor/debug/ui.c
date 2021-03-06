#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

/**************************************work******************************/
//TODO1
static int cmd_si(char* args){  //si N
  int n=0;
  if(args==NULL) n=1; // if no parameter with si,set the default n to 1
  else{
    while(*args==' ') args++;//omit space character
    while(*args){
      if(isdigit(args[0])) 
        n=n*10+args[0]-'0';
      else{
        printf("usage si N\n");
        break;
      }
      args++;
    }
  }
  cpu_exec(n);
  return 0;
}

extern void isa_reg_display();
extern void wp_show();
static int cmd_info(char*args){
  while(args!=NULL&&*args==' ') args++;
  if(args==NULL)
    printf("usage \"info r\" or \"info w\"\n");
  else if(*args=='r')
    isa_reg_display();
  else if(*args=='w'){
    wp_show();
  }
  else
    printf("invalid SUBCOMMAND,should be 'r' or 'w'\n");
  return 0;
}

static int cmd_x(char* args){ // X N expr
  int n;
  vaddr_t va;
  bool valid=true;
  if(sscanf(args,"%d",&n)!=1)
    printf("usage x N expr\n");
  else{
    while(*args==' ') args++;
    while(isdigit(*args)) args++;
    va=expr(args,&valid);
    if(!valid) printf("usage x N expr\n");
    else 
      for(int i=0;i<n;i++)
        printf("%0x10x  %0x10x\n",va+i*4,vaddr_read(va+i*4,4));
  }
  return 0;
}

static int cmd_p(char* args){
  bool valid=true;
  uint32_t val=expr(args,&valid);
  if(valid==false) printf("invalid expression\n");
  else printf("0x%08x\n",val);
  return 0;
}

extern WP* new_wp();
static int cmd_w(char* args){
  if(args==NULL){
    printf("usage w EXPR\n");
    return -1;
  }
  Assert(strlen(args)<32,"exp is too long!\n");
  bool valid=true;
  uint32_t val=expr(args,&valid);
  if(!valid){
    printf("invalid expression in watchpoint!\n");
    return -1;
  }
  WP* pwp=new_wp();
  strcpy(pwp->exp_str,args); //fill in exp_str
  pwp->old_val=val; //fill_in old_val
  return 0;
}

extern void free_wp(int wp_num);
static int cmd_d(char* args){
  if(args==NULL){
    printf("usage: d N\n");
    return -1;
  }
  int num;
  if(sscanf(args,"%d",&num)!=1){
    printf("usage: d N\n");
    return -1;
  }
  free_wp(num);
  return 0;
}
static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Single instruction",cmd_si},
  { "info", "register info or watchpoint info",cmd_info},
  { "x", "print memrory info", cmd_x},
  { "p", "get the value of expression", cmd_p},
  { "w", "stop when a given exp is changed",cmd_w},
  { "d", "delete a watch point", cmd_d},
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
