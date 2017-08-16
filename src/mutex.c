#include "mutex.h"

//properly instantiates a new mutex
mutex mutex_create()
{
    mutex m;
    m.lock_var=0;
    m.value=0;
    m.thread_id=0;
    return m;
}

//lock a mutex, in future should also note thread id of locker
uint32_t mutex_lock(mutex m)
{
    //atomic exchange instruction for use as mutex
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

//unlock a mutex, in future should also only allow locker to unlock
uint32_t mutex_unlock(mutex m)
{
    //atomic exchange instruction for use as mutex
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
