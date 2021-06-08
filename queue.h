/* queue.h
   The queue is implemented as a linked list where the data field
   in the queue element is a pointer to an unspecified type.
*/

#ifndef __queue__
#define __queue__

typedef void *QueueData;

typedef 
  struct queueElem {
    QueueData item;    
    struct queueElem *next;
  } queueElem, *link;


typedef struct QueueRecord {
  
  // To be decided by the student

} QueueRecord, *Queue;

Queue     newQueue();                   
void      enqueue(Queue q, QueueData it); 
QueueData dequeue(Queue q);               
QueueData first(Queue q);               
int       qLength(Queue q);     
void      qitReset(Queue q);
int       qitMore(Queue q);              
QueueData qitNext(Queue q);              

#endif
