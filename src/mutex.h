#ifndef _MUTEX_INC
#define _MUTEX_INC

#include <stdint.h>

typedef struct
{
    uint32_t lock_var;
    uint32_t value;
}__attribute__((packed)) mutex;

uint32_t mutex_lock(mutex m);
uint32_t mutex_unlock(mutex m);

#endif
