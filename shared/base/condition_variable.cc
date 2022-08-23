#include "condition_variable.h"
#include "logger.h"

condition_variable::condition_variable(mutex_lock& lock)
    :user_mutex_(lock.get_mutex())
{
#if defined(PLATFORM_LINUX)
    int rv = pthread_cond_init(&condition_, nullptr);
#elif defined(PLATFORM_WINDOWS)
    int rv = InitializeConditionVariable(&condition_);
#endif
    CHECK_EQ(rv, 0);
}

condition_variable::~condition_variable()
{
#if defined(PLATFORM_LINUX)
    int rv = pthread_cond_destroy(&condition_);
#elif defined(PLATFORM_WINDOWS)
    int rv = 0;
#endif
    CHECK_EQ(rv, 0);
}

void condition_variable::wait()
{
#if defined(PLATFORM_LINUX)
    int rv = pthread_cond_wait(&condition_, user_mutex_);
#elif defined(PLATFORM_WINDOWS)
    int rv = SleepConditionVariableCS(&condition_, user_mutex_);
#endif
    CHECK_EQ(rv, 0);
}

void condition_variable::broadcast()
{
#if defined(PLATFORM_LINUX)
    int rv = pthread_cond_broadcast(&condition_);
#elif defined(PLATFORM_WINDOWS)
    int rv = WakeAllConditionVariable(&condition_);
#endif
    CHECK_EQ(rv, 0);
}

void condition_variable::signal()
{
#if defined(PLATFORM_LINUX)
    int rv = pthread_cond_signal(&condition_);
#elif defined(PLATFORM_WINDOWS)
    int rv = WakeConditionVariable(&condition_);
#endif
    CHECK_EQ(rv, 0);
}