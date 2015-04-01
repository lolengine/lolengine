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

//BaseThreadManager -----------------------------------------------------------
BaseThreadManager::BaseThreadManager(int thread_count)
{
    m_thread_min = thread_count;
    m_thread_count = thread_count;
}

BaseThreadManager::BaseThreadManager(int thread_min, int thread_count)
{
    m_thread_min = thread_min;
    m_thread_count = thread_count;
}

BaseThreadManager::~BaseThreadManager()
{
    Stop();
}

//Initialize, Ticker::Ref and start the thread
bool BaseThreadManager::Start()
{
    if (m_threads.count() > 0)
        return false;

    //Add minimum threads
    m_threads.reserve(m_thread_count);
    AddThreads(m_thread_min);

    return true;
}

//Stop the threads
bool BaseThreadManager::Stop()
{
    if (m_threads.count() <= 0)
        return false;

    //Stop all threads
    StopThreads((int)m_threads.count());

    return true;
}

//----
void BaseThreadManager::AddThreads(int nb)
{
    //Don't add threads if not availables
#if LOL_FEATURE_THREADS
    //Spawn worker threads and ...
    for (int i = 0; i < nb; i++)
        m_threads << new thread(std::bind(&BaseThreadManager::BaseThreadWork, this));

    //... Wait for their readiness.
    for (int i = 0; i < m_thread_count; i++)
        m_spawnqueue.pop();
#endif //LOL_FEATURE_THREADS
}

//----
void BaseThreadManager::StopThreads(int nb)
{
    //Don't stop threads if not availables
#if LOL_FEATURE_THREADS
    //Signal worker threads for completion and ...
    ThreadJob stop_job(ThreadJobType::THREAD_STOP);
    for (int i = 0; i < nb; i++)
        m_jobqueue.push(&stop_job);

    //... Wait for them to quit.
    for (int i = 0; i < nb; i++)
        m_donequeue.pop();
#endif //LOL_FEATURE_THREADS
}

//Work stuff
bool BaseThreadManager::AddWork(ThreadJob* job, bool force)
{
    if (!force)
        return m_jobqueue.try_push(job);
    m_jobqueue.push(job);
    return true;
}

//----
bool BaseThreadManager::FetchResult(array<ThreadJob*>& results)
{
    ThreadJob* result;
    while (m_resultqueue.try_pop(result))
        results << result;
    return results.count() > 0;
}

//Base thread work function
void BaseThreadManager::BaseThreadWork()
{
#if LOL_FEATURE_THREADS
    //Register that the thread has started
    m_spawnqueue.push(ThreadStatus::THREAD_STARTED);
    for ( ; ; )
#endif //LOL_FEATURE_THREADS
    {
        //Try to retrieve a job
        ThreadJob* job = m_jobqueue.pop();
        //Stop thread
        if (job->GetJobType() == ThreadJobType::THREAD_STOP)
        {
#if LOL_FEATURE_THREADS
            break;
#endif //LOL_FEATURE_THREADS
        }
        //Or work
        else if (*job == ThreadJobType::WORK_TODO)
        {
            if (job->DoWork())
                job->SetJobType(ThreadJobType::WORK_SUCCESSED);
            else
                job->SetJobType(ThreadJobType::WORK_FAILED);
            m_resultqueue.push(job);
        }
    }
#if LOL_FEATURE_THREADS
    //Register that the thread has stopped
    m_donequeue.push(ThreadStatus::THREAD_STOPPED);
#endif //LOL_FEATURE_THREADS
}

//----
void BaseThreadManager::TickGame(float seconds)
{
    Entity::TickGame(seconds);

    //Start if needed
    Start();

    //Dispatch work task
    while (m_job_dispatch.count() > 0 && AddWork(m_job_dispatch.last()))
        m_job_dispatch.pop();

    //Execute one task per frame if thread are not available
#if !LOL_FEATURE_THREADS
    BaseThreadWork();
#endif // !LOL_FEATURE_THREADS

    array<ThreadJob*> result;
    //Fetch and treat results
    if (FetchResult(result))
    {
        for (int i = 0; i < result.count(); i++)
        {
            ThreadJob* job = result[i];
            TreatResult(job);
        }
    }

    //Resize thread count if needed
    if (m_threads.count() > m_jobqueue.count() && m_threads.count() > m_thread_min)
        StopThreads((int)(m_threads.Count() - m_thread_min));
    else if (m_threads.count() < m_jobqueue.count())
        AddThreads((int)(lol::min(m_jobqueue.count(), (ptrdiff_t)m_thread_count) - m_threads.count()));
}

} /* namespace lol */
