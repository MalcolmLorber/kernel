#ifndef _MUTEX_INC
#define _MUTEX_INC

#include <stdint.h>

//definition of kernel-level mutexes, to be used in process management and other areas

//mutex structure
typedef struct
{
    uint32_t lock_var;  //the actual variable that acts as a lock
    uint32_t value;     //to allow multi-locks
    uint32_t thread_id; //the id of the thread that locked the mutex (TO BE IMPLEMENTED)
}__attribute__((packed)) mutex;

//mutex handling funcitons, see mutex.c
mutex mutex_create();
uint32_t mutex_lock(mutex m);
uint32_t mutex_unlock(mutex m);

#endif
