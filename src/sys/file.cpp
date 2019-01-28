//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if __ANDROID__
#   include <sys/types.h>
#   include <android/asset_manager_jni.h>
#endif

#if defined(_WIN32)
#   define WIN32_LEAN_AND_MEAN 1
#   include <windows.h>
#   undef WIN32_LEAN_AND_MEAN
#else
#   include <dirent.h>
#endif

#if defined HAVE_UNISTD_H
#   include <unistd.h>
#endif

#include <atomic>
#include <string>
#include <algorithm>
#include <sys/stat.h>

namespace lol
{

#if __ANDROID__
extern AAssetManager *g_assets;
#endif

//---------------
class FileData
{
    friend class File;

    FileData()
      : m_refcount(0),
        m_type(StreamType::File)
    { }

    void Open(StreamType stream)
    {
        if (m_type == StreamType::File ||
            m_type == StreamType::FileBinary)
            return;
        m_type = stream;
        switch (stream.ToScalar())
        {
#if __ANDROID__
        /* FIXME: no modes, no error checking, no nothing */
#elif HAVE_STDIO_H
            case StreamType::StdIn:  m_fd = stdin;  break;
            case StreamType::StdOut: m_fd = stdout; break;
            case StreamType::StdErr: m_fd = stderr; break;
#endif
            default: break;
        }
    }

    void Open(std::string const &file, FileAccess mode, bool force_binary)
    {
        m_type = (force_binary) ? (StreamType::FileBinary) : (StreamType::File);
#if __ANDROID__
        ASSERT(g_assets);
        m_asset = AAssetManager_open(g_assets, file.c_str(), AASSET_MODE_UNKNOWN);
#elif HAVE_STDIO_H
        /* FIXME: no modes, no error checking, no nothing */
        stat(file.c_str(), &m_stat);
        std::string access(mode == FileAccess::Write ? "w" : "r");
        if (force_binary)
            access += "b";
        m_fd = fopen(file.c_str(), access.c_str());
#endif
    }

    inline bool IsValid() const
    {
#if __ANDROID__
        return !!m_asset;
#elif HAVE_STDIO_H
        return !!m_fd;
#else
        return false;
#endif
    }

    void Close()
    {
        if (m_type != StreamType::File &&
            m_type != StreamType::FileBinary)
            return;
#if __ANDROID__
        if (m_asset)
            AAsset_close(m_asset);
        m_asset = nullptr;
#elif HAVE_STDIO_H
        if (m_fd)
            fclose(m_fd);
        m_fd = nullptr;
#endif
    }

    int Read(uint8_t *buf, int count)
    {
#if __ANDROID__
        return AAsset_read(m_asset, buf, count);
#elif HAVE_STDIO_H
        size_t done = fread(buf, 1, count, m_fd);
        if (done <= 0)
            return -1;

        return (int)done;
#else
        return 0;
#endif
    }

    std::string ReadString()
    {
        array<uint8_t> buf;
        buf.resize(BUFSIZ);
        std::string ret;
        while (IsValid())
        {
            int done = Read(&buf[0], buf.count());

            if (done <= 0)
                break;

            size_t oldsize = ret.length();
            ret.resize(oldsize + done);
            memcpy(&ret[oldsize], &buf[0], done);

            buf.resize(buf.count() * 3 / 2);
        }
        return ret;
    }

    int Write(void const *buf, int count)
    {
#if __ANDROID__
        //return AAsset_read(m_asset, buf, count);
        return 0;
#elif HAVE_STDIO_H
        size_t done = fwrite(buf, 1, count, m_fd);
        if (done <= 0)
            return -1;

        return (int)done;
#else
        return 0;
#endif
    }

    long int GetPosFromStart()
    {
#if __ANDROID__
        return 0;
#elif HAVE_STDIO_H
        return ftell(m_fd);
#else
        return 0;
#endif
    }

    void SetPosFromStart(long int pos)
    {
#if __ANDROID__
        //NOT IMPLEMENTED
#elif HAVE_STDIO_H
        fseek(m_fd, pos, SEEK_SET);
#else
        //NOT IMPLEMENTED
#endif
    }

    long int size()
    {
#if __ANDROID__
        return 0;
#elif HAVE_STDIO_H
        return m_stat.st_size;
#else
        return 0;
#endif
    }

    long int GetModificationTime()
    {
#if __ANDROID__
        return 0;
#elif HAVE_STDIO_H
        return (long int)m_stat.st_mtime;
#else
        return 0;
#endif
    }

    //-----------------------
#if __ANDROID__
    AAsset *m_asset;
#elif HAVE_STDIO_H
    FILE *m_fd;
#endif
    std::atomic<int> m_refcount;
    StreamType m_type;
    struct stat m_stat;
};

//-- FILE --
File::File()
  : m_data(new FileData)
{
    ++m_data->m_refcount;
}

//--
File::File(File const &that)
  : m_data(that.m_data)
{
    ++m_data->m_refcount;
}

//--
File &File::operator =(File const &that)
{
    if (this == &that)
        return *this;

    /* FIXME: this needs auditing */
    int refcount = --m_data->m_refcount;
    if (refcount == 0)
    {
        m_data->Close();
        delete m_data;
    }

    m_data = that.m_data;
    ++m_data->m_refcount;

    return *this;
}

//--
File::~File()
{
    int refcount = --m_data->m_refcount;
    if (refcount == 0)
    {
        m_data->Close();
        delete m_data;
    }
}

//--
void File::Open(StreamType stream)
{
    m_data->Open(stream);
}

//--
void File::Open(std::string const &file, FileAccess mode, bool force_binary)
{
    m_data->Open(file, mode, force_binary);
}

//--
bool File::IsValid() const
{
    return m_data->IsValid();
}

//--
void File::Close()
{
    m_data->Close();
}

//--
int File::Read(uint8_t *buf, int count)
{
    return m_data->Read(buf, count);
}

//--
std::string File::ReadString()
{
    return m_data->ReadString();
}

//--
int File::Write(void const *buf, int count)
{
    return m_data->Write(buf, count);
}

//--
int File::Write(std::string const &buf)
{
    return m_data->Write(buf.c_str(), (int)buf.length());
}

//--
long int File::GetPosFromStart()
{
    return m_data->GetPosFromStart();
}

//--
void File::SetPosFromStart(long int pos)
{
    m_data->SetPosFromStart(pos);
}

//--
long int File::size()
{
    return m_data->size();
}

//--
long int File::GetModificationTime()
{
    return m_data->GetModificationTime();
}

//---------------
class DirectoryData
{
    friend class Directory;

    DirectoryData() : m_type(StreamType::File)
    {
#if __ANDROID__
        /* FIXME: not implemented */
#elif defined(_WIN32)
        m_handle = INVALID_HANDLE_VALUE;
#elif HAVE_STDIO_H
        m_dd = nullptr;
#endif
    }

    void Open(std::string const &directory, FileAccess mode)
    {
        UNUSED(mode); /* FIXME */

        m_type = StreamType::File;
#if __ANDROID__
        /* FIXME: not implemented */
#elif defined(_WIN32)
        m_directory = directory;
        std::string filter = m_directory + "*";
        std::replace(filter.begin(), filter.end(), '/', '\\');
        WIN32_FIND_DATA FindFileData;
        m_handle = FindFirstFile(filter.c_str(), &FindFileData);
        stat(directory.c_str(), &m_stat);
#elif HAVE_STDIO_H
        m_dd = opendir(directory.c_str());
        stat(directory.c_str(), &m_stat);
#endif
    }

    void Close()
    {
        if (m_type != StreamType::File)
            return;

        if (IsValid())
        {
#if __ANDROID__
            /* FIXME: not implemented */
#elif defined(_WIN32)
            FindClose(m_handle);
#elif HAVE_STDIO_H
            closedir(m_dd);
#endif
        }

#if __ANDROID__
        /* FIXME: not implemented */
#elif defined(_WIN32)
        m_handle = INVALID_HANDLE_VALUE;
#elif HAVE_STDIO_H
        m_dd = nullptr;
#endif
    }

    bool GetContentList(array<std::string>* files, array<std::string>* directories)
    {
        if (!IsValid())
            return false;

#if __ANDROID__
        /* FIXME: not implemented */
#elif defined(_WIN32)
        std::string filter = m_directory + "*";
        std::replace(filter.begin(), filter.end(), '/', '\\');
        WIN32_FIND_DATA find_data;
        HANDLE handle = FindFirstFile(filter.c_str(), &find_data);
        bool file_valid = (handle != INVALID_HANDLE_VALUE);

        while (file_valid)
        {
            if (find_data.cFileName[0] != '.')
            {
                // We have a directory
                if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if (directories)
                        *directories << std::string(find_data.cFileName);
                }
                else
                {
                    if (files)
                        *files << std::string(find_data.cFileName);
                }
            }
            //Go for next one
            file_valid = !!FindNextFile(m_handle, &find_data);
        }
#elif HAVE_STDIO_H
        /* FIXME: not implemented */
#endif
        return ((files && files->count()) || (directories && directories->count()));
    }

    inline bool IsValid() const
    {
#if __ANDROID__
        /* FIXME: not implemented */
#elif defined(_WIN32)
        return (m_handle != INVALID_HANDLE_VALUE);
#elif HAVE_STDIO_H
        return !!m_dd;
#else
        return false;
#endif
    }

    long int GetModificationTime()
    {
#if __ANDROID__
        return 0;
#elif HAVE_STDIO_H
        return (long int)m_stat.st_mtime;
#else
        return 0;
#endif
    }

#if __ANDROID__
    /* FIXME: not implemented */
#elif defined(_WIN32)
    HANDLE m_handle;
    std::string m_directory;
#elif HAVE_STDIO_H
    DIR *m_dd;
#endif
    std::atomic<int> m_refcount;
    StreamType m_type;
    struct stat m_stat;
};

//-- DIRECTORY --
Directory::Directory(std::string const &name)
  : m_data(new DirectoryData),
    m_name(name + "/")
{
    ++m_data->m_refcount;
}

//--
Directory::Directory(Directory const &that)
  : m_data(that.m_data),
    m_name(that.m_name)
{
    ++m_data->m_refcount;
}

//--
Directory &Directory::operator =(Directory const &that)
{
    if (this == &that)
        return *this;

    /* FIXME: this needs auditing */
    int refcount = --m_data->m_refcount;
    if (refcount == 0)
    {
        m_data->Close();
        delete m_data;
    }

    m_data = that.m_data;
    m_name = that.m_name;
    ++m_data->m_refcount;

    return *this;
}

//--
Directory::~Directory()
{
    int refcount = --m_data->m_refcount;
    if (refcount == 0)
    {
        m_data->Close();
        delete m_data;
    }
}

//--
void Directory::Open(FileAccess mode)
{
    return m_data->Open(m_name, mode);
}

//--
bool Directory::IsValid() const
{
    return m_data->IsValid();
}

//--
void Directory::Close()
{
    m_data->Close();
}

//--
bool Directory::GetContent(array<std::string>* files, array<Directory>* directories)
{
    array<std::string> sfiles, sdirectories;
    bool found_some = m_data->GetContentList(&sfiles, &sdirectories);
    UNUSED(found_some);

    if (directories)
        for (int i = 0; i < sdirectories.count(); i++)
            directories->push(Directory(m_name + sdirectories[i]));

    if (files)
        for (int i = 0; i < sfiles.count(); i++)
            files->push(m_name + sfiles[i]);

    return (files && files->count()) || (directories || directories->count());
}

//--
bool Directory::GetContent(array<std::string>& files, array<Directory>& directories)
{
    return GetContent(&files, &directories);
}

//--
bool Directory::GetContent(array<Directory>& directories)
{
    return GetContent(nullptr, &directories);
}

//--
bool Directory::GetContent(array<std::string>& files)
{
    return GetContent(&files, nullptr);
}

//--
std::string Directory::GetName()
{
    return m_name;
}

//--
long int Directory::GetModificationTime()
{
    return m_data->GetModificationTime();
}

//--
std::string Directory::GetCurrent()
{
    std::string ret;
#if __ANDROID__
    /* FIXME: not implemented */
#elif defined(_WIN32)
    TCHAR buff[MAX_PATH * 2];
    GetCurrentDirectory(MAX_PATH, buff);
    ret = buff;
    std::replace(ret.begin(), ret.end(), '\\', '/');
#elif HAVE_STDIO_H
    /* FIXME: not implemented */
#endif
    return ret;
}

//--
bool Directory::SetCurrent(std::string directory)
{
#if __ANDROID__
    /* FIXME: not implemented */
#elif defined(_WIN32)
    std::string result = directory;
    std::replace(result.begin(), result.end(), '/', '\\');
    return !!SetCurrentDirectory(result.c_str());
#elif HAVE_UNISTD_H
    chdir(directory.c_str());
#endif
    return false;
}

} /* namespace lol */

