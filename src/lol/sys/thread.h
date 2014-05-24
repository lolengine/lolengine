//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Threading classes
// ---------------------
//

#if !defined __LOL_SYS_THREAD_H__
#define __LOL_SYS_THREAD_H__

#if defined __CELLOS_LV2__
#   include "platform/ps3/threadbase.h"
#else
#   include "sys/threadbase.h"
#endif

namespace lol
{

class Mutex : public MutexBase
{
public:
    Mutex() : MutexBase() {}
};

template<typename T, int N = 128> class Queue : public QueueBase<T, N>
{
public:
    Queue() : QueueBase<T, N>() {}
};

#if LOL_FEATURE_THREADS
class Thread : ThreadBase
{
public:
    Thread(void *(*fn)(void *), void *data) : ThreadBase(fn, data) {}
    virtual ~Thread() {}
};

struct ThreadCommand
{
    enum Value
    {
        THREAD_STARTED,
        THREAD_STOPPED,
        WORK_TODO,
        WORK_DONE,
        THREAD_STOP,

        MAX
    }
    m_value;

    inline ThreadCommand()                  : m_value(ThreadCommand::MAX) {}
    inline ThreadCommand(Value v)           : m_value(v) {}
    bool operator==(const ThreadCommand& v) { return m_value == v.m_value; }
};

template<typename T1>
struct JobCommand : public ThreadCommand
{
    inline JobCommand()
      : ThreadCommand(ThreadCommand::WORK_TODO)
    {}

    inline JobCommand(Value v)
      : ThreadCommand(v)
    {}

    inline JobCommand(T1 const &data)
      : ThreadCommand(ThreadCommand::WORK_TODO),
        m_data(data)
    {}

    inline JobCommand(Value v, T1 const &data)
      : ThreadCommand(v),
        m_data(data)
    {}

    inline void SetData(T1 const &data)
    {
        m_data = data;
    }

    inline T1 GetData()
    {
        return m_data;
    }

private:
    T1 m_data;
};

template<typename T1, typename T2> class ThreadManager
{
public:
    ThreadManager(int thread_count)
    {
        m_thread_count = thread_count;
    }

    //Initialize and start the thread
    bool Start()
    {
        if (m_threads.Count() > 0)
            return false;

        /* Spawn worker threads and wait for their readiness. */
        m_threads.Resize(m_thread_count);
        for (int i = 0; i < m_thread_count; i++)
            m_threads[i] = new Thread(BaseThreadWork, this);
        for (int i = 0; i < m_thread_count; i++)
            m_spawnqueue.Pop();

        return true;
    }

    bool AddWork(const JobCommand<T1>& job)
    {
        if (m_jobqueue.TryPush(job))
            return true;
        return false;
    }
    bool FetchResult(Array<JobCommand<T2> >& results)
    {
        JobCommand<T2> result;
        while (m_resultqueue.TryPop(result))
            results << result;
        return results.Count() > 0;
    }

    static void *BaseThreadWork(void* data)
    {
        ThreadManager *that = (ThreadManager *)data;
        that->m_spawnqueue.Push(ThreadCommand::THREAD_STARTED);
        for ( ; ; )
        {
            JobCommand<T1> job = that->m_jobqueue.Pop();
            if (job == ThreadCommand::THREAD_STOP)
                break;
            else if (job == ThreadCommand::WORK_TODO)
            {
                JobCommand<T2> result(ThreadCommand::WORK_DONE);
                if (that->DoThreadWork(job, result))
                    that->m_resultqueue.Push(result);
            }
        }
        that->m_donequeue.Push(ThreadCommand::THREAD_STOPPED);
        return NULL;
    }

    virtual bool DoThreadWork(JobCommand<T1>& job, JobCommand<T2>& result)
    {
        return false;
    }

    //Stop the thread
    bool Stop()
    {
        if (m_threads.Count() <= 0)
            return false;

        /* Signal worker threads for completion and wait for
         * them to quit. */
        for (int i = 0; i < m_thread_count; i++)
            m_jobqueue.Push(ThreadCommand::THREAD_STOP);
        for (int i = 0; i < m_thread_count; i++)
            m_donequeue.Pop();

        return true;
    }

protected:
    /* Worker threads */
    int                     m_thread_count;
    Array<Thread*>          m_threads;
    Queue<ThreadCommand>    m_spawnqueue, m_donequeue;
    Queue<JobCommand<T1> >  m_jobqueue;
    Queue<JobCommand<T2> >  m_resultqueue;
};
#endif

} /* namespace lol */

#endif // __LOL_SYS_THREAD_H__

