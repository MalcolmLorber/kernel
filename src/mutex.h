#ifndef _MUTEX_INC
#define _MUTEX_INC

#include <stdint.h>

//definition of kernel-level mutexes, to be used in process management and other areas

//mutex structure
typedef struct
{
    uint32_t lock_var;
    uint32_t value;
    uint32_t thread_id;
}__attribute__((packed)) mutex;

//mutex handling funcitons, see mutex.c
mutex mutex_create();
uint32_t mutex_lock(mutex m);
uint32_t mutex_unlock(mutex m);

#endif
