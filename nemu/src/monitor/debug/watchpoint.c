#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
/********************work***********/
//API to allocate and free wp
WP* new_wp(){
  Assert(free_,"no more wp space in wp_pool!\n");
  WP* res=free_;
  free_=free_->next;
  res->next=NULL;//get a free wp
  if(head==NULL) return head=res;
  //add res to head tail
  WP* pwp=head;
  while(pwp->next) pwp=pwp->next;
  pwp->next=res;
  return res;
}
void free_wp(int wp_num){
  if(head==NULL)
    printf("no watchpoints!\n");
  else if(head->NO==wp_num){ //special case
    if(free_==NULL) {
      free_=head;
      head=head->next;
      free_->next=NULL;
    }
    else{
      WP* p=free_;
      while(p->next) p=p->next; //get the last free wp
      p->next=head;  // add head to free list
      head=head->next;
      p->next->next=NULL;
    }
  }
  else{
    WP* ph=head;
    while(ph->next&&ph->next->NO!=wp_num) ph=ph->next;
    if(ph->next==NULL){
      printf("no wp match number:%d\n",wp_num);
      return ;
    }
    WP* res=ph->next;
    ph->next=res->next;
    res->next=NULL;
    if(free_==NULL) free_=res;
    else {
      WP* pf=free_;
      while(pf->next) pf=pf->next;
      pf->next=res;
    }
  }
}

void wp_show(){
  printf("%5s %10s %30s\n","WP_NO","OLD_VAL","EXPRESSION");
  WP* pwp=head;
  while(pwp){
    printf("%-5d 0x%-10x %-30s\n",pwp->NO,pwp->old_val,pwp->exp_str);
    pwp=pwp->next;
  }
}

bool wp_check(){
  bool ans=true;
  WP* pwp=head;
  while(pwp!=NULL){
    bool valid=true;
    uint32_t val=expr(pwp->exp_str,&valid);
    assert(valid);
    if(val!=pwp->old_val){
      printf("WP_NO:%2d OLD_VAL:0x%08d NEW_VAL:0x%08d EXP:%s\n",pwp->NO,pwp->old_val,val,pwp->exp_str);
      pwp->old_val=val;
      ans=false;
    }
    pwp=pwp->next;
  }
  return ans;
}