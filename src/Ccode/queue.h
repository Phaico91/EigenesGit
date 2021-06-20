#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue Queue;
Queue *queue_new(unsigned long queuesize);
void queue_delete(Queue *q);
bool queue_isempty(const Queue *q);
void queue_add(Queue *q, void *elem);
void *queue_get(Queue *q);

#endif
