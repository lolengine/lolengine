//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//                     2014 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

namespace lol
{

#if LOL_FEATURE_THREADS
BaseThreadManager::BaseThreadManager(int thread_count)
{
    m_thread_count = thread_count;
}
BaseThreadManager::~BaseThreadManager()
{
    Stop();
}

//Initialize, Ticker::Ref and start the thread
bool BaseThreadManager::Start()
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

//Stop the threads
bool BaseThreadManager::Stop()
{
    if (m_threads.Count() <= 0)
        return false;

    /* Signal worker threads for completion and wait for
        * them to quit. */
    ThreadJob stop_job(ThreadJobType::THREAD_STOP);
    for (int i = 0; i < m_thread_count; i++)
        m_jobqueue.Push(&stop_job);
    for (int i = 0; i < m_thread_count; i++)
        m_donequeue.Pop();

    return true;
}
    
//Work stuff
bool BaseThreadManager::AddWork(ThreadJob* job)
{
    if (m_jobqueue.TryPush(job))
        return true;
    return false;
}
bool BaseThreadManager::FetchResult(Array<ThreadJob*>& results)
{
    ThreadJob* result;
    while (m_resultqueue.TryPop(result))
        results << result;
    return results.Count() > 0;
}

//Base thread work function
void *BaseThreadManager::BaseThreadWork(void* data)
{
    BaseThreadManager *that = (BaseThreadManager *)data;
    that->m_spawnqueue.Push(ThreadStatus::THREAD_STARTED);
    for ( ; ; )
    {
        ThreadJob* job = that->m_jobqueue.Pop();
        if (job->GetJobType() == ThreadJobType::THREAD_STOP)
            break;
        else if (*job == ThreadJobType::WORK_TODO)
        {
            if (job->DoWork())
                job->SetJobType(ThreadJobType::WORK_DONE);
            else
                job->SetJobType(ThreadJobType::WORK_FAILED);
            that->m_resultqueue.Push(job);
        }
    }
    that->m_donequeue.Push(ThreadStatus::THREAD_STOPPED);
    return NULL;
}

void BaseThreadManager::TickGame(float seconds)
{
    //Start if needed
    Start();

    //Dispatch work task
    while (m_job_dispatch.Count() > 0 && AddWork(m_job_dispatch.Last()))
        m_job_dispatch.Pop();

    Array<ThreadJob*> result;
    //Fetch and treat results
    if (FetchResult(result))
    {
        for (int i = 0; i < result.Count(); i++)
        {
            ThreadJob* job = result[i];
            if (job->GetJobType() == ThreadJobType::WORK_DONE)
                TreatResult(job);
        }
    }
}
#endif //LOL_FEATURE_THREADS

} /* namespace lol */
