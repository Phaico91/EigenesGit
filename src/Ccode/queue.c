#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"

struct Queue
{
 void **queuespace;  /* the space to store the queue elements */
 unsigned long enqueueindex, /* entry into which element is to be enqued */
               dequeueindex, /* last element of queue */
               queuesize,    /* size of the queue */
               noofelements; /* no ofelements between enqueueindex+1
                                and dequeindex */
};

/*
  The following function delivers an empty queue with a reservoir of
  \texttt{size} elements to be stored in the queue. The
  reservoir can, if necessary, be enlarged.
*/

Queue *queue_new(unsigned long queuesize)
{
  Queue *q;

  assert(queuesize > 0);
  q = malloc(sizeof *q);
  assert(q != NULL);
  q->queuespace = malloc(queuesize * sizeof(void *));
  assert(q->queuespace != NULL);
  q->noofelements = 0;
  q->queuesize = queuesize;
  q->dequeueindex = q->enqueueindex = queuesize - 1;
  return q;
}

/*
  The following function frees the space required for the queue.
*/

void queue_delete(Queue *q)
{
  free(q->queuespace);
  free(q);
}

/*
  The following function returns true iff the queue is empty.
*/

bool queue_isempty(const Queue *q)
{
  return (q->noofelements == 0) ? true : false;
}

/*
  The following function resizes the queue by doubling the
  space reservoir.
*/

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

static void extendqueuesize(Queue *q)
{
  unsigned long addconst, idx, newsize;

  addconst = q->queuesize;
  newsize = q->queuesize + addconst;
  q->queuespace = realloc(q->queuespace,newsize * sizeof(void *));
  assert(q->queuespace != NULL);
  assert(q->enqueueindex == q->dequeueindex);
  assert(addconst > 0);
  for (idx=q->queuesize-1; idx>q->enqueueindex; idx--)
  {
    q->queuespace[idx+addconst] = q->queuespace[idx];
  }
  q->enqueueindex += addconst;
  q->queuesize = newsize;
}

/*
  The following function adds an element \texttt{elem} to the end of
  the queue.
*/

void queue_add(Queue *q, void *elem)
{
  if (q->noofelements == q->queuesize)
  {
    extendqueuesize(q);
  }
  q->noofelements++;
  q->queuespace[q->enqueueindex] = elem;
  if (q->enqueueindex > 0)
  {
    q->enqueueindex--;
  } else
  {
    q->enqueueindex = q->queuesize - 1; /* dequeuindex < queuesize-1 */
  }
}

/*
  The following function removes the element \texttt{elem} from the
  start of the queue.
*/

void *queue_get(Queue *q)
{
  void *value;

  assert(q->noofelements > 0);
  q->noofelements--;
  value = q->queuespace[q->dequeueindex];
  if (q->dequeueindex > 0)
  {
    q->dequeueindex--;
  } else
  {
    q->dequeueindex = q->queuesize - 1;  /* != enqueueindex, since at least
                                            one elem */
  }
  return value;
}
