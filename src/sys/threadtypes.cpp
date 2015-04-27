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
void DefaultThreadManager::AddJob(ThreadJob* job)
{
    DispatchJob(job);
}
bool DefaultThreadManager::GetWorkResult(array<ThreadJob*>& results)
{
    results += m_job_result;
    m_job_result.empty();
    return results.count() > 0;
}

//FileUpdateTesterJob ---------------------------------------------------------
class FileUpdateTesterJob : public ThreadJob
{
    friend class FileUpdateTester;
public:
    char const *GetName() { return "<FileUpdateTesterJob>"; }
    FileUpdateTesterJob()
        : ThreadJob(ThreadJobType::NONE) { }
    FileUpdateTesterJob(String path)
        : ThreadJob(ThreadJobType::WORK_TODO)
    {
        m_path = path;
    }
    String& GetPath()   { return m_path; }
    long int GetTime()  { return m_time; }
    bool HasUpdated()   { return m_updated; }
    void Restart()
    {
        SetJobType(ThreadJobType::WORK_TODO);
        m_updated = false;
    }

protected:
    virtual bool DoWork()
    {
        array<String> pathlist = System::GetPathList(m_path);
        File f;
        for (String path : pathlist)
        {
            f.Open(path, FileAccess::Read);
            if (f.IsValid())
            {
                long int new_time = f.GetModificationTime();
                if (!m_ready)
                {
                    m_time = new_time;
                    m_ready = true;
                }
                else if (new_time > m_time)
                {
                    m_time = new_time;
                    m_updated = true;
                }
                return true;
            }
        }
        return false;
    }

    //-----------------
    bool        m_ready = false;
    String      m_path = String();
    long int    m_time = 0;
    bool        m_updated = false;
};

//FileUpdateTester ------------------------------------------------------------
FileUpdateTester::~FileUpdateTester()
{
    ASSERT(!m_files.count(), "Files need to be unregistered before destroying FileUpdateTester");
}

//File interface --------------------------------------------------------------
FileUpdateTester::Status* FileUpdateTester::RegisterFile(String const& path)
{
    DispatchJob(new FileUpdateTesterJob(path));
    m_files[path] = new FileUpdateTester::Status();
    return m_files[path];
}

void FileUpdateTester::UnregisterFile(String const& path)
{
    ASSERT(m_files.has_key(path));
    delete m_files[path];
    m_files.remove(path);
}

void FileUpdateTester::UnregisterFile(FileUpdateTester::Status*& status)
{
    ASSERT(status);
    array<String> keys = m_files.keys();
    for (String key : keys)
    {
        if (m_files[key] == status)
        {
            UnregisterFile(key);
            status = nullptr;
            return;
        }
    }
    ASSERT(false, "Status wasn't found");
}

//-----------------------------------------------------------------------------
void FileUpdateTester::TickGame(float seconds)
{
    //Reset update for this frame
    array<String> keys = m_files.keys();
    for (String key : keys)
        m_files[key]->SetUpdated(false);

    super::TickGame(seconds);

    if (!GetDispatchCount() && m_job_done.count())
    {
        if (m_frame_count++ < m_frame_skip)
            return;
        m_frame_count = 0;

        DispatchJob(m_job_done);
        m_job_done.empty();
    }
}

//-----------------------------------------------------------------------------
void FileUpdateTester::TreatResult(ThreadJob* result)
{
    FileUpdateTesterJob* job = static_cast<FileUpdateTesterJob*>(result);
    if (job->HasUpdated())
    {
        m_files[job->GetPath()]->SetTime(job->GetTime());
        m_files[job->GetPath()]->SetUpdated(true);
    }
    job->Restart();
    m_job_done << job;
}

//AsyncImageJob ---------------------------------------------------------------
class AsyncImageJob : public ThreadJob
{
public:
    char const *GetName() { return "<AsyncImageJob>"; }
    AsyncImageJob()
        : ThreadJob(ThreadJobType::NONE)
    {
        m_path = String();
    }
    AsyncImageJob(String path)
        : ThreadJob(ThreadJobType::WORK_TODO)
    {
        m_path = path;
    }
    String const& GetPath() { return m_path; }
    Image const& GetImage() { return m_image; }

protected:
    virtual bool DoWork()
    {
        return m_image.Load(m_path.C());
    }

    String  m_path;
    Image   m_image;
};

//-----------------------------------------------------------------------------
Image* AsyncImageLoader::Load(const lol::String& path)
{
    //Create a job
    AsyncImageJob* job = new AsyncImageJob(path);
    //Create a dummy image
    Image* image = new Image(m_dummy_image);
    //Link the two
    m_images[path] = image;
    //Add job
    DispatchJob(job);
    //return Dummy image
    return image;
}

//-----------------------------------------------------------------------------
bool AsyncImageLoader::CheckStatus(Image* image)
{
    if (m_loaded_images.find(image) != INDEX_NONE)
    {
        m_loaded_images.remove_item(image);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
void AsyncImageLoader::TreatResult(ThreadJob* result)
{
    AsyncImageJob* job = dynamic_cast<AsyncImageJob*>(result);
    ASSERT(job);
    //Copy image if work is OK
    if (job->GetJobType() == ThreadJobType::WORK_SUCCEEDED)
    {
        Image* src = m_images[job->GetPath()];
        m_images.remove(job->GetPath());
        src->Copy(job->GetImage());
        m_loaded_images.push_unique(src);
    }
    else
    {
        msg::error("AsyncImageJob FAILED. See load image error above.\n");
    }
    //Delete all that
    delete(result);
}
