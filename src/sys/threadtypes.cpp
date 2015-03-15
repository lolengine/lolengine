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

using namespace lol;

//DefaultThreadManager --------------------------------------------------------
bool DefaultThreadManager::AddJob(ThreadJob* job)
{
    return AddWork(job);
}
bool DefaultThreadManager::GetWorkResult(array<ThreadJob*>& results)
{
    results += m_job_result;
    m_job_result.Empty();
    return results.Count() > 0;
}

//FileUpdateTesterJob ---------------------------------------------------------
bool FileUpdateTesterJob::DoWork()
{
    File f;
    f.Open(m_path, FileAccess::Read);
    if (!f.IsValid())
        return false;
    if (!m_ready)
        m_time = f.GetModificationTime();
    else
    {
        long int new_time = f.GetModificationTime();
        if (new_time > m_time)
            m_updated = true;
    }
    return true;
}

//FileUpdateTester ------------------------------------------------------------
FileUpdateTester::Status* FileUpdateTester::RegisterFile(String const& path)
{
    m_job_dispatch << new FileUpdateTesterJob(path);
    m_files[path] = new FileUpdateTester::Status();
    return m_files[path];
}

//-------------------------------------------------------------------------
void FileUpdateTester::TickGame(float seconds)
{
    super::TickGame(seconds);

    if (!m_job_dispatch.count() && m_job_done.count())
    {
        m_job_dispatch = m_job_done;
        m_job_done.empty();
    }
}

//-------------------------------------------------------------------------
void FileUpdateTester::TreatResult(ThreadJob* result)
{
    FileUpdateTesterJob* job = static_cast<FileUpdateTesterJob*>(result);
    if (job->HasUpdated())
    {
        m_files[job->GetPath()]->m_updated = true;
        job->Restart();
        m_job_done << job;
    }
}
