#ifndef _QUEUE_INC
#define _QUEUE_INC

#include "PCB.h"

struct node;

typedef struct
{
    struct node *front,*rear;
} queue;

void queue_push(queue q, process *proc);
void queue_pop(queue q);
process* queue_front(queue q);
void create(queue q);
#endif
