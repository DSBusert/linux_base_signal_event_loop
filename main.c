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
#ifdef STACK
  return 1;
#else
  add_timer_event(1, my_call_back,
      "this my fucking call back function\n");
  return 0;
#endif
}

int mcb(void *arg)
{
  printf("%s\n", (char*)arg);
#ifdef STACK
  return 0;
#else
  add_timer_event(1, mcb, "this second cb\n");
#endif
  return 0;
}

int main()
{

  timer_event_loop_init();
#ifndef STACK
  add_timer_event(1, my_call_back,
      "this my fucking call back function\n");
  add_timer_event(1, mcb, "this second cb\n");
#elif defined(STACK)
  add_timer_event(1, 1, my_call_back,
      "this my fucking call back function\n");
  add_timer_event(1, 1, mcb, "this second cb\n");
#endif
  while(1);
  return 0;
}
