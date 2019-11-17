#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,

  /* TODO: Add more token types */
  /* opnd type */
  TK_NUM,TK_HEX,
  /* operator type */
  TK_MINUS,TK_DEREF
};

static struct rule { //词法规则
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  //arithmatic op
  {"\\+", '+'},         // plus
  {"\\-", '-'},         // sub
  {"\\*", '*'},         // multiply
  {"/",   '/'},         // divid
  //opnd
  {"[0-9]+",TK_NUM},
  {"0x[0-9A-Za-z]+",TK_HEX}, //hex
  //logic op
  {"==", TK_EQ},        // equal
  //bracket
  {"\\(", '('},         //left  bracket
  {"\\)", ')'}          //right bracket
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {}; 
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          /****************work*****************/
          case TK_NOTYPE: break;
          case TK_NUM:
          case TK_HEX:
            Assert(substr_len<32,"digit string is too long!\n");
            strncpy(tokens[nr_token].str,substr_start,substr_len);
          case '+':
          case '-':
          case '*':
          case '/':
          case '(':
          case ')':
            tokens[nr_token++].type=rules[i].token_type;break;
          default: TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


/********************work****************************/
bool check_parentheses(int p,int q){ 
  if(tokens[p].type!='('||tokens[q].type!=')') return false;
  int bracket=0;//bracket++ when facing '(' and bracket-- facing ')'
  for(int i=p;i<=q;i++){
    if(tokens[i].type=='(') bracket++;
    else if(tokens[i].type==')') bracket--;
    if(bracket<0) return false;
  }
  return bracket==0;
}

int get_pri(int type){
  switch(type){
    case TK_MINUS:
    case TK_DEREF:
      return 2;
    case '*':
    case '/':
      return 3;
    case '+':
    case '-':
      return 4;
    default:
      panic("unknown type\n");
      return -1;
  }
}

bool is_right_bound(int type){
  return type==TK_DEREF||type==TK_MINUS;
}

int get_master_op(int p,int q){
  int cnt=0;
  int res=-1;
  int pri1,pri2;
  for(int i=p;i<=q;i++){
    switch (tokens[i].type){
      case '(':cnt++;break;
      case ')':cnt--;break;
      case '+':case '-':case '*':case '/':  //arithmatic op
      case TK_DEREF:case TK_MINUS:  // special op
        if(cnt!=0) break;
        pri1=get_pri(tokens[i].type);
        pri2=get_pri(tokens[res].type);
        if(res==-1||pri1>pri2||(pri1==pri2&&!is_right_bound(tokens[i].type))) {res=i;break;}
      default:
        break;
    }
  }
  return res;
}

bool isopnd(int idx){
  Assert(idx>=0&&idx<nr_token,"idx of isopnd should in range [0,nrtoken)\n");
  int type=tokens[idx].type;
  return type==TK_NUM||type==TK_HEX;
}

uint32_t eval(int p,int q,bool* success){
  if (p > q) {
    *success=false;
    return -1;
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    if(tokens[p].type==TK_NUM){
      uint32_t val;
      sscanf(tokens[p].str,"%d",&val);
      return val;
    }else if(tokens[p].type==TK_HEX){
      uint32_t val;
      sscanf(tokens[p].str,"%x",&val);
      return val;
    }else{
      *success=false;
      Log("only one token but it's not opnd\n");
      return -1; 
    }
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1,success);
  }
  else {
    int master_op_idx = get_master_op(p,q);
    if(tokens[master_op_idx].type==TK_MINUS){
      Assert(master_op_idx==p,"master_op should be equal to p\n");
      uint32_t val=eval(master_op_idx+1,q,success);
      return -val;
    }
    else if(tokens[master_op_idx].type==TK_DEREF){
      Assert(master_op_idx==p,"master_op should be equal to p\n");
      uint32_t addr=eval(master_op_idx+1,q,success);
      return vaddr_read(addr,4);
    }
    else{
      uint32_t val1 = eval(p, master_op_idx - 1,success);
      if(*success==false) return -1;
      uint32_t val2 = eval(master_op_idx + 1, q,success);
      if(*success==false) return -1;
      switch (tokens[master_op_idx].type) {
        case '+': return val1 + val2;
        case '-': return val1 - val2;
        case '*': return val1 * val2;
        case '/': return val1 / val2;
        default: assert(0);return -1;
      }
    }
  }
}
uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  //deal with TK_DEREF and TK_MINUS
  for(int i=0;i<nr_token;i++){
    if(tokens[i].type=='*'&&(i==0||!isopnd(i-1)))
      tokens[i].type=TK_DEREF;
    else if(tokens[i].type=='-'&&(i==0||!isopnd(i-1)))
      tokens[i].type=TK_MINUS;
  }

  return eval(0,nr_token-1,success);
}
