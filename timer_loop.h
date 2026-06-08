#if !defined(_TIMER_LOOP_H_)
#define _TIMER_LOOP_H_
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define STACK
typedef struct timer_node{
  
  unsigned int sec_tval;
  unsigned int remain_sec;

  int (*cb)(void* );
  void *arg; //remain if the argment has been free,the will be dranger

  struct timer_node* next;

}timer_node_t;
#ifdef STACK
timer_node_t *add_timer_event(unsigned int sec_val, unsigned int remain_sec,
  int (*cb)(void*), void* arg);

#else
timer_node_t *add_timer_event(unsigned int sec_val, int (*cb)(void*), void* arg);
#endif
void timer_event_loop_init();

#endif
