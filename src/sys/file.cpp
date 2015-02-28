//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if __ANDROID__
#   include <sys/types.h>
#   include <android/asset_manager_jni.h>
#endif

#if defined(_WIN32)
#   define WIN32_LEAN_AND_MEAN 1
#   include <windows.h>
#else
#   include <dirent.h>
#endif

#include <atomic>
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
        }
    }

    void Open(String const &file, FileAccess mode, bool force_binary)
    {
        m_type = (force_binary) ? (StreamType::FileBinary) : (StreamType::File);
#if __ANDROID__
        ASSERT(g_assets);
        m_asset = AAssetManager_open(g_assets, file.C(), AASSET_MODE_UNKNOWN);
#elif HAVE_STDIO_H
        /* FIXME: no modes, no error checking, no nothing */
        stat(file.C(), &m_stat);
        String access = (mode == FileAccess::Write) ? ("w") : ("r");
        if (force_binary) access += "b";
        m_fd = fopen(file.C(), access.C());
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

    String ReadString()
    {
        array<uint8_t> buf;
        buf.Resize(BUFSIZ);
        String ret;
        while (IsValid())
        {
            int done = Read(&buf[0], (int)buf.Count());

            if (done <= 0)
                break;

            int oldsize = (int)ret.Count();
            ret.Resize(oldsize + done);
            memcpy(&ret[oldsize], &buf[0], done);

            buf.Resize(buf.Count() * 3 / 2);
        }
        return ret;
    }

    int Write(uint8_t const *buf, int count)
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

    int WriteString(const String &buf)
    {
        return Write((uint8_t const *)buf.C(), (int)buf.Count());
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

    long int GetSize()
    {
#if __ANDROID__
        return 0;
#elif HAVE_STDIO_H
        return m_stat.st_size;
#else
        return 0;
#endif
    }


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
void File::Open(String const &file, FileAccess mode, bool force_binary)
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
String File::ReadString()
{
    return m_data->ReadString();
}

//--
int File::Write(uint8_t const *buf, int count)
{
    return m_data->Write(buf, count);
}

//--
int File::WriteString(const String &buf)
{
    return m_data->WriteString(buf);
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
long int File::GetSize()
{
    return m_data->GetSize();
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

    void Open(String const &directory, FileAccess mode)
    {
        m_type = StreamType::File;
#if __ANDROID__
        /* FIXME: not implemented */
#elif defined(_WIN32)
        m_directory = directory;
        String filter = m_directory + String("*");
        filter.Replace('/', '\\', true);
        WIN32_FIND_DATA FindFileData;
        m_handle = FindFirstFile(filter.C(), &FindFileData);
#elif HAVE_STDIO_H
        m_dd = opendir(directory.C());
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

    bool GetContentList(array<String>* files, array<String>* directories)
    {
        if (!IsValid())
            return false;

#if __ANDROID__
        /* FIXME: not implemented */
#elif defined(_WIN32)
        String filter = m_directory + String("*");
        filter.Replace('/', '\\', true);
        WIN32_FIND_DATA find_data;
        HANDLE handle = FindFirstFile(filter.C(), &find_data);
        bool file_valid = (handle != INVALID_HANDLE_VALUE);

        while (file_valid)
        {
            if (find_data.cFileName[0] != '.')
            {
                // We have a directory
                if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if (directories)
                        *directories << String(std::string(find_data.cFileName).c_str());
                }
                else
                {
                    if (files)
                        *files << String(find_data.cFileName);
                }
            }
            //Go for next one
            file_valid = (bool)FindNextFile(m_handle, &find_data);
        }
#elif HAVE_STDIO_H
        /* FIXME: not implemented */
#endif
        return ((files && files->Count()) || (directories && directories->Count()));
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

#if __ANDROID__
    /* FIXME: not implemented */
#elif defined(_WIN32)
    HANDLE m_handle;
    String m_directory;
#elif HAVE_STDIO_H
    DIR *m_dd;
#endif
    std::atomic<int> m_refcount;
    StreamType m_type;
};

//-- DIRECTORY --
Directory::Directory(String const &name)
  : m_data(new DirectoryData),
    m_name(name + String("/"))
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
bool Directory::GetContent(array<String>* files, array<Directory>* directories)
{
    array<String> sfiles, sdirectories;
    bool found_some = m_data->GetContentList(&sfiles, &sdirectories);

    if (directories)
        for (int i = 0; i < sdirectories.Count(); i++)
            directories->Push(Directory(m_name + sdirectories[i]));

    if (files)
        for (int i = 0; i < sfiles.Count(); i++)
            files->Push(m_name + sfiles[i]);

    return (files && files->Count()) || (directories || directories->Count());
}

//--
bool Directory::GetContent(array<String>& files, array<Directory>& directories)
{
    return GetContent(&files, &directories);
}

//--
bool Directory::GetContent(array<Directory>& directories)
{
    return GetContent(nullptr, &directories);
}

//--
bool Directory::GetContent(array<String>& files)
{
    return GetContent(&files, nullptr);
}

//--
String Directory::GetName()
{
    return m_name;
}

//--
String Directory::GetCurrent()
{
    String result;
#if __ANDROID__
    /* FIXME: not implemented */
#elif defined(_WIN32)
    TCHAR buff[MAX_PATH * 2];
    GetCurrentDirectory(MAX_PATH, buff);
    result = buff;
    result.Replace('\\', '/', true);
#elif HAVE_STDIO_H
    /* FIXME: not implemented */
#endif
    return result;
}

//--
bool Directory::SetCurrent(String directory)
{
#if __ANDROID__
    /* FIXME: not implemented */
#elif defined(_WIN32)
    String result = directory;
    result.Replace('/', '\\', true);
    return (bool)SetCurrentDirectory(result.C());
#elif HAVE_STDIO_H
    /* FIXME: not implemented */
#endif
    return false;
}

} /* namespace lol */

