#ifndef DEMO_RPC_CONDITION_VARIABLE_H
#define DEMO_RPC_CONDITION_VARIABLE_H
#include "lock.h"

class condition_variable
{
public:
    explicit condition_variable(mutex_lock& lock);

    ~condition_variable();

    void wait();

    void broadcast();

    void signal();
private:
#if defined(PLATFORM_LINUX)
    pthread_cond_t condition_;
    pthread_mutex_t* user_mutex_;
#elif defined(PLATFORM_WINDOWS)
#endif
};
#endif //DEMO_RPC_CONDITION_VARIABLE_H
