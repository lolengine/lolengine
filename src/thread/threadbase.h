//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The ThreadBase class
// --------------------
//

#if !defined __LOL_THREADBASE_H__
#define __LOL_THREADBASE_H__

#if defined __linux__ || defined __native_client__
#   include <pthread.h>
#elif defined _WIN32
#   include <windows.h>
#else
#   error No threading support yet :(
#endif

namespace lol
{

class MutexBase
{
public:
    MutexBase()
    {
#if defined __linux__ || defined __native_client__
        pthread_mutex_init(&m_mutex, NULL);
#elif defined _WIN32
        InitializeCriticalSection(&m_mutex);
#endif
    }

    ~MutexBase()
    {
#if defined __linux__ || defined __native_client__
        pthread_mutex_destroy(&m_mutex);
#elif defined _WIN32
        DeleteCriticalSection(&m_mutex);
#endif
    }

    void Lock()
    {
#if defined __linux__ || defined __native_client__
        pthread_mutex_lock(&m_mutex);
#elif defined _WIN32
        EnterCriticalSection(&m_mutex);
#endif
    }

    void Unlock()
    {
#if defined __linux__ || defined __native_client__
        pthread_mutex_unlock(&m_mutex);
#elif defined _WIN32
        LeaveCriticalSection(&m_mutex);
#endif
    }

private:
#if defined __linux__ || defined __native_client__
    pthread_mutex_t m_mutex;
#elif defined _WIN32
    CRITICAL_SECTION m_mutex;
#endif
};

class ConditionBase
{
public:
    ConditionBase()
    {
#if defined __linux__ || defined __native_client__
        pthread_mutex_init(&m_mutex, NULL);
        pthread_cond_init(&m_cond, NULL);
#endif
    }

    ~ConditionBase()
    {
#if defined __linux__ || defined __native_client__
        pthread_cond_destroy(&m_cond);
        pthread_mutex_destroy(&m_mutex);
#endif
    }

    void Acquire()
    {
#if defined __linux__ || defined __native_client__
        pthread_mutex_lock(&m_mutex);
#endif
    }

    void Release()
    {
#if defined __linux__ || defined __native_client__
        pthread_mutex_unlock(&m_mutex);
#endif
    }

    void Wait()
    {
#if defined __linux__ || defined __native_client__
        pthread_cond_wait(&m_cond, &m_mutex);
#endif
    }

    void Notify()
    {
#if defined __linux__ || defined __native_client__
        pthread_cond_signal(&m_cond);
#endif
    }

private:
#if defined __linux__ || defined __native_client__
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
#endif
};

class ThreadBase
{
public:
    ThreadBase(void *(*fn)(void *), void *data)
    {
#if defined __linux__ || defined __native_client__
        pthread_create(&m_thread, NULL, fn, data);
#elif defined _WIN32
        m_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fn, data, 0, &m_tid);
#endif
    }

    virtual ~ThreadBase()
    {
#if defined __linux__ || defined __native_client__
        pthread_join(m_thread, NULL);
#elif defined _WIN32
        WaitForSingleObject(m_thread, INFINITE);
#endif
    }

private:
#if defined __linux__ || defined __native_client__
    pthread_t m_thread;
#elif defined _WIN32
    HANDLE m_thread;
    DWORD m_tid;
#endif
};

} /* namespace lol */

#endif // __LOL_THREADBASE_H__

