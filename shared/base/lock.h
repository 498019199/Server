#ifndef __BASE_MUTEX__H__
#define __BASE_MUTEX__H__

#if defined(PLATFORM_LINUX)
#include <pthread.h>
#elif defined(PLATFORM_WINDOWS)
#include <windows.h>
#endif

#if defined(PLATFORM_LINUX)
    typedef pthread_mutex_t mutex_t;
#elif defined(PLATFORM_WINDOWS)
    typedef CRITICAL_SECTION mutex_t;
#endif

class mutex_lock
{
public:
    mutex_lock()
    {
#if defined(PLATFORM_LINUX)
        pthread_mutex_init(&m_mutex, nullptr);
#elif defined(PLATFORM_WINDOWS)
        ::InitializeCriticalSectionAndSpinCount(&m_mutex, 2000);
#endif
    }

    ~mutex_lock()
    {
#if defined(PLATFORM_LINUX)
        pthread_mutex_destroy(&m_mutex);
#elif defined(PLATFORM_WINDOWS)
        ::DeleteCriticalSection(&m_mutex);
#endif
    }

    void lock()
    {
#if defined(PLATFORM_LINUX)
        pthread_mutex_lock(&m_mutex);
#elif defined(PLATFORM_WINDOWS)
        ::EnterCriticalSection(&m_mutex);
#endif
    }

    void unlock()
    {
#if defined(PLATFORM_LINUX)
        pthread_mutex_unlock(&m_mutex);
#elif defined(PLATFORM_WINDOWS)
        ::LeaveCriticalSection(&m_mutex);
#endif
    }

    bool try_lock()
    {
#if defined(PLATFORM_LINUX)
        return pthread_mutex_trylock(&m_mutex) == 0;
#elif defined(PLATFORM_WINDOWS)
        return (::TryEnterCriticalSection(&m_mutex) != FALSE);
#endif
    }

    mutex_t* get_mutex()
    {
        return &m_mutex;
    }

private:
    mutex_t m_mutex{};
};

class lock:public mutex_lock
{
public:
    lock(){}
    ~lock(){}

    void acquire() { lock();}
    void release() { unlock(); }
    bool trylock() { return try_lock(); }
    void assert_acquired() const {}
private:
    mutex_lock mutex_;
};

class auto_lock
{
public:
    auto_lock(lock& l)
        : lock_(l)
    {
        lock_.assert_acquired();
        lock_.acquire();
    }

    ~auto_lock()
    {
        lock_.assert_acquired();
        lock_.release();
    }
private:
    lock& lock_;
};

class auto_unlock
{
public:
    auto_unlock(lock& l)
        : lock_(l)
    {
        lock_.assert_acquired();
        lock_.release();
    }

    ~auto_unlock()
    {
        lock_.acquire();
    }
private:
    lock& lock_;
};
#endif//__BASE_MUTEX__H__