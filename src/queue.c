#include "queue.h"

#define NULL 0

struct node
{
    process* proc;
    struct node *next;
};

struct node* node_create(process *proc)
{
    struct node* n = malloc(sizeof(struct node));
    n->next = NULL;
    n->proc = proc;
    return n;
}

void queue_push(queue q, process *proc)
{
    struct node* n = node_create(proc);
    if(q.rear!=NULL)
    {
        q.rear->next = n;
        q.rear = n;
    }
    else
    {
        q.rear = q.front = n;
    }
}

void queue_pop(queue q)
{
    if(q.front!=NULL)
    {
        struct node* f =q.front->next;
        //free(q.front)
        q.front = f;
        if(q.front==NULL)
        {
            q.rear=NULL;
        }
    }
}

process* queue_front(queue q)
{
    return q.front->proc;
}

void create(queue q)
{
    q.front = q.rear = NULL;
}
