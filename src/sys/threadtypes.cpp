//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//            © 2014—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
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
    m_job_result.clear();
    return results.count() > 0;
}

//FileUpdateTesterJob ---------------------------------------------------------
class FileUpdateTesterJob : public ThreadJob
{
    friend class FileUpdateTester;
public:
    std::string GetName() const { return "<FileUpdateTesterJob>"; }

    FileUpdateTesterJob()
      : ThreadJob(ThreadJobType::NONE)
    {
    }

    FileUpdateTesterJob(std::string const &path)
      : ThreadJob(ThreadJobType::WORK_TODO),
        m_path(path)
    {
    }

    std::string const &GetPath() { return m_path; }
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
        array<std::string> pathlist = sys::get_path_list(m_path);
        File f;
        for (auto const &path : pathlist)
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
    bool m_ready = false;
    std::string m_path;
    long int m_time = 0;
    bool m_updated = false;
};

//FileUpdateTester ------------------------------------------------------------
FileUpdateTester::~FileUpdateTester()
{
    ASSERT(!m_files.size(), "Files need to be unregistered before destroying FileUpdateTester");
}

//File interface --------------------------------------------------------------
FileUpdateTester::Status* FileUpdateTester::RegisterFile(std::string const& path)
{
    DispatchJob(new FileUpdateTesterJob(path));
    m_files[path] = new FileUpdateTester::Status();
    return m_files[path];
}

void FileUpdateTester::UnregisterFile(std::string const& path)
{
    ASSERT(has_key(m_files, path));
    delete m_files[path];
    m_files.erase(path);
}

void FileUpdateTester::UnregisterFile(FileUpdateTester::Status*& status)
{
    ASSERT(status);
    for (auto const &key : keys(m_files))
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
    // Reset update for this frame
    for (auto &kv : m_files)
        kv.second->SetUpdated(false);

    super::TickGame(seconds);

    if (!GetDispatchCount() && m_job_done.count())
    {
        if (m_frame_count++ < m_frame_skip)
            return;
        m_frame_count = 0;

        DispatchJob(m_job_done);
        m_job_done.clear();
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
    std::string GetName() const { return "<AsyncImageJob>"; }

    AsyncImageJob()
      : ThreadJob(ThreadJobType::NONE)
    {
    }

    AsyncImageJob(std::string const &path)
      : ThreadJob(ThreadJobType::WORK_TODO),
        m_path(path)
    {
    }

    std::string const& GetPath() { return m_path; }
    Image const& GetImage() { return m_image; }

protected:
    virtual bool DoWork()
    {
        return m_image.load(m_path);
    }

    std::string m_path;
    Image m_image;
};

//-----------------------------------------------------------------------------
Image* AsyncImageLoader::Load(std::string const &path)
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
        m_images.erase(job->GetPath());
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
