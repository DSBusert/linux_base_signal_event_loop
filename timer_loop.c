#include "timer_loop.h"

#define DEBUG 0

#if defined(DEBUG) && DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT
#endif
static timer_node_t *timer_header = NULL;
#ifdef STACK
void timer_stack()
{
  timer_node_t **pp = &timer_header, *p = (*pp);
  while(*pp)
  {
    if((*pp)->sec_tval == 0 && (*pp)->cb != NULL)
    {
      int ret = (*pp)->cb((*pp)->arg);
      if(ret > 0)
      {
        (*pp)->sec_tval = (*pp)->remain_sec;
      }
      else
      {
       // pp = &(*pp)->net; /*will segmentation fault*/
        *pp = p->next;
        free(p);
      }
    }
    else
    {
      (*pp)->sec_tval--;
      pp = &(*pp)->next;
    }
  }
}
#endif

void timer_stick_loop()
{
  timer_node_t **pp = &timer_header;
DEBUG_PRINT("Head P:%p Head next:%p\n", timer_header, timer_header->next);
  while(*pp)//loop all node
  {
    if((*pp)->sec_tval == 0)
    {
      if((*pp)->cb)
        (*pp)->cb( (*pp)->arg );
    //delate node
      if(*pp == timer_header)
      {
      pp = &(*pp)->next;
DEBUG_PRINT("%d \n", __LINE__);
        timer_node_t *cur = (*pp);
DEBUG_PRINT("%d cur:%p\n", __LINE__, cur);
        (*pp) = cur->next;
DEBUG_PRINT("%d *pp:%p\n", __LINE__, *pp);
        free(cur);
DEBUG_PRINT("*free node:%p, pp:%p, (*pp)->next:%p\n", cur, *pp, *pp ? (*pp)->next:NULL);
        continue;
      }
      timer_node_t *cur = *pp;
      *pp = cur->next;
      free(cur);
DEBUG_PRINT("*free node:%p, pp:%p, (*pp)->next:%p\n", cur, *pp, (*pp)->next);
    }
    else
    {
      (*pp)->sec_tval--;
      pp = &(*pp)->next;
    }
  }
}

#ifdef STACK
timer_node_t *add_timer_event(unsigned int sec_val, unsigned int remain_val,
  int (*cb)(void*), void* arg)
#else
timer_node_t *add_timer_event(unsigned int sec_val, int (*cb)(void*), void* arg)
#endif
{
  timer_node_t *p = (timer_node_t *)malloc(sizeof(timer_node_t));

  if(p == NULL)
  {
    perror("malloc faile\n");
    return NULL;
  }
DEBUG_PRINT("new node:%p\n", p);
#ifdef STACK
  p->remain_sec = remain_val;
#endif
  p->sec_tval = sec_val;
  p->cb = cb;
  p->arg = arg;
//head insert
  p->next = timer_header;
  timer_header = p;

//last insert
#if 0 
printf("insert start\n");
  timer_node_t *node = timer_header;
  while(node->next != NULL) node = node->next;
  printf("node:%p noed->next:%p\n", node, node->next);
  p->next = NULL;
  node->next = p;
printf("insert end\n");
#endif
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
