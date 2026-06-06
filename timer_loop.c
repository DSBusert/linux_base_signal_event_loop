#include "timer_loop.h"


static timer_node_t *timer_header = NULL;
//#define STACK
#define HEAD_INSERT
#ifdef STACK
void timer_stack()
{
	timer_node_t **pp = &timer_header;

	while(*pp)
	{
		timer_node_t *p = (*pp);
		if((*pp)->sec_tval == 0)
		{
			int ret = (*pp)->cb((*pp)->arg);
			if(ret == 0)
			{
				pp = &(*pp)->next;
				free(p);
			}
			else
			{
				(*pp)->sec_tval = (*pp)->remain_sec;
			}
		}
		else
		{
			(*pp)->sec_tval--;
		}
	  pp = &(*pp)->next;
	}
}
#else

void timer_stick_loop()
{
  timer_node_t *pp = timer_header;
  timer_node_t * node = pp;
  printf("Head P:%p Head next:%p\n", timer_header, timer_header->next);
  while(pp)//loop all node
  {
    if(node->sec_tval == 0)
    {
      if(node->cb)
        node->cb( node->arg );
      pp = pp->next;
      free(node);
    }
    else
    {
      pp->sec_tval--;
      pp = pp->next;
    }
  }
}

#endif
timer_node_t *add_timer_event(unsigned int sec_val, unsigned int remain_sec, int (*cb)(void*), void* arg)
{
  timer_node_t *p = (timer_node_t *)malloc(sizeof(timer_node_t));

  if(p == NULL)
  {
    perror("malloc faile\n");
    return NULL;
  }
  p->remain_sec = remain_sec;
  p->sec_tval = sec_val;
  p->cb = cb;
  p->arg = arg;
  
printf("insert start\n");
#ifdef HEAD_INSERT
  p->next = timer_header;
  timer_header = p;
#else
  //last insert
  timer_node_t *node = timer_header;
  while(1) {
	  if(node->next == NULL) break;
  
	  node = node->next;
  }
  printf("node:%p noed->next:%p\n", node, node->next);
  p->next = NULL;
  node->next = p;
 #endif
printf("insert end\n");
  return p;
}

void sec_timer_base(int signal)
{
  //printf("signal is %d\n", signal);
#ifdef STACK
	timer_stack();
#else
  timer_stick_loop();
#endif
  alarm(1);
}

void timer_event_loop_init()
{
  signal(SIGALRM, sec_timer_base);
  alarm(1);
}
