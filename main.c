#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#include "timer_loop.h"
//extern timer_node_t *add_timer_event(unsigned int sec_val, int (*cb)(void*), void* arg);
//extern void timer_event_loop_init();

int my_call_back(void* arg)
{
  printf("%s\n", (char *)arg);
 #ifndef stack
  add_timer_event(1, 1, my_call_back,
      "this my fucking call back function\n");
 #endif
  return 1;
}

int main()
{

  timer_event_loop_init();

  add_timer_event(1, 1, my_call_back,
      "this my fucking call back function\n");
  while(1);
  return 0;
}
