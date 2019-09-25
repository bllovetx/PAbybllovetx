#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

#define NR_WP 32
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char args[640];
  uint32_t old_val;
  bool wp_Enb;
  bool wp_used;

  /* TODO: Add more members if necessary */


} WP;
extern WP wp_pool[];
#endif
