//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2014—2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

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
        m_threads[i] = new thread(std::bind(&BaseThreadManager::BaseThreadWork,
                                            this));
    for (int i = 0; i < m_thread_count; i++)
        m_spawnqueue.pop();

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
        m_jobqueue.push(&stop_job);
    for (int i = 0; i < m_thread_count; i++)
        m_donequeue.pop();

    return true;
}

//Work stuff
bool BaseThreadManager::AddWork(ThreadJob* job)
{
    if (m_jobqueue.try_push(job))
        return true;
    return false;
}

bool BaseThreadManager::FetchResult(array<ThreadJob*>& results)
{
    ThreadJob* result;
    while (m_resultqueue.try_pop(result))
        results << result;
    return results.Count() > 0;
}

//Base thread work function
void BaseThreadManager::BaseThreadWork()
{
    m_spawnqueue.push(ThreadStatus::THREAD_STARTED);
    for ( ; ; )
    {
        ThreadJob* job = m_jobqueue.pop();
        if (job->GetJobType() == ThreadJobType::THREAD_STOP)
            break;
        else if (*job == ThreadJobType::WORK_TODO)
        {
            if (job->DoWork())
                job->SetJobType(ThreadJobType::WORK_DONE);
            else
                job->SetJobType(ThreadJobType::WORK_FAILED);
            m_resultqueue.push(job);
        }
    }
    m_donequeue.push(ThreadStatus::THREAD_STOPPED);
}

void BaseThreadManager::TickGame(float seconds)
{
    Entity::TickGame(seconds);

    //Start if needed
    Start();

    //Dispatch work task
    while (m_job_dispatch.Count() > 0 && AddWork(m_job_dispatch.Last()))
        m_job_dispatch.pop();

    array<ThreadJob*> result;
    //Fetch and treat results
    if (FetchResult(result))
    {
        for (int i = 0; i < result.Count(); i++)
        {
            ThreadJob* job = result[i];
            if (job->GetJobType() == ThreadJobType::WORK_DONE)
            {
                job->SetJobType(ThreadJobType::WORK_FETCHED);
                TreatResult(job);
            }
        }
    }
}
#endif //LOL_FEATURE_THREADS

} /* namespace lol */
