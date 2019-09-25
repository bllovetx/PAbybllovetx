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
WP* new_wp(){
	assert(free_!=NULL);
	WP *wp_return=free_;
	free_=free_->next;
	return wp_return;
}
void free_wp(WP *wp){
	WP* wp_pre=NULL;
	for(int i=0;i<NR_WP;i++){
		if(wp_pool[i].next==wp)wp_pre=&wp_pool;
	}
	assert(wp_pre!=NULL);
	wp_pre->next=wp->next;
	wp->next=free_;
	free_=wp;
}
