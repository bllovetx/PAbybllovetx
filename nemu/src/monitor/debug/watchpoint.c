#include "monitor/watchpoint.h"
#include "monitor/expr.h"

//#define NR_WP 32
//remove static add extern in w....h
WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;
WP* new_wp();
void free_wp(WP* wp);
void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
	wp_pool[i].wp_used=false;
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(){
	assert(free_!=NULL);
	WP *wp_return=free_;
	free_=free_->next;		// get newwp from free
	wp_return->next=head;
	head=wp_return;			// insert newwp at head
	wp_return->wp_Enb=true;
	wp_return->wp_used=true;// change flags
	return wp_return;
}
void free_wp(WP *wp){
	/* find ancestor */
	WP* wp_pre=NULL;
	for(int i=0;i<NR_WP;i++){ 
		if(wp_pool[i].next==wp)wp_pre=&wp_pool[i];
	}
	if( wp_pre==NULL ){		// head
		assert(head==wp);
		head=wp->next;
	}
	else{					// not head
		wp_pre->next=wp->next;
	}
	wp->next=free_;			// insert wp at free_
	free_=wp;
	wp->wp_used=false;		// change flag
}
void wp_used_display(){
	printf("NO		Enb		old value(dex)		old value(hex)		expr\n");
	for(int i=0;i<NR_WP;i++){
		if(wp_pool[i].wp_used){
			char enb='n';
			if(wp_pool[i].wp_Enb)enb='y';
			printf("%2d		%3c		%14u		    0x%08x		%s\n",wp_pool[i].NO,enb,wp_pool[i].old_val,wp_pool[i].old_val,wp_pool[i].args);
		}
	}
}
bool check_watch(){
	bool watch_changed=false;
	for(WP* wp_i=head;wp_i!=NULL;wp_i=wp_i->next){
	    if( (wp_i->wp_Enb) && (wp_i->wp_used) ){
	   	 bool success=true;
	   	 uint32_t new_wval=expr(wp_i->args,&success);
	   	 assert(success);
	   	 if( wp_i->old_val!=new_wval ){
	   		 printf("watchpoint %d changed:%s\nold value:%d(%#x)    new value:%d(%#x)\n",
	   				 wp_i->NO,wp_i->args,wp_i->old_val,wp_i->old_val,new_wval,new_wval);
	   		 watch_changed=true;
	   		 wp_i->old_val=new_wval;
	   	 }
	    }
	}
	return watch_changed;
}
