#include "mutex.h"

mutex mutex_create()
{
    mutex m;
    m.lock_var=0;
    m.value=0;
    m.thread_id=0;
    return m;
}

uint32_t mutex_lock(mutex m)
{
    asm volatile
    (
     "mov $1, %%eax;       \
      xchg %[lock], %%eax; \
      mov %%eax, %[value]"
     :[value] "=r" (m.value)
     :[lock] "r" (m.lock_var)
    );
    return m.value;
}

uint32_t mutex_unlock(mutex m)
{
    asm volatile
    (
     "mov $0, %%eax;       \
      xchg %[lock], %%eax; \
      mov %%eax, %[value]"
     :[value] "=r" (m.value)
     :[lock] "r" (m.lock_var)
    );
    return m.value;
}
