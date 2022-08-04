#ifndef __BASE_MUTEX__H__
#define __BASE_MUTEX__H__
#include <pthread.h>

class mutex
{
public:
    mutex()
    {
        pthread_mutex_init(&m_mutex, nullptr);
    }

    ~mutex()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    void lock()
    {
        pthread_mutex_lock(&m_mutex);
    }

    void unlock()
    {
        pthread_mutex_unlock(&m_mutex);
    }

    pthread_mutex_t* get_mutex()
    {
        return &m_mutex;
    }

private:
  pthread_mutex_t m_mutex;
};

#endif//__BASE_MUTEX__H__