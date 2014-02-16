//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#if __CELLOS_LV2__
#   include <sys/paths.h>
#   include <cell/cell_fs.h>
#elif __ANDROID__
#   include <sys/types.h>
#   include <android/asset_manager_jni.h>
#endif

#include "core.h"

namespace lol
{

#if __ANDROID__
extern AAssetManager *g_assets;
#endif

class FileData
{
    friend class File;

    FileData() : m_type(StreamType::File) { }

    void Open(StreamType stream)
    {
        if (m_type == StreamType::File)
            return;
        m_type = stream;
        switch((int)stream)
        {
#if __CELLOS_LV2__
        /* FIXME: no modes, no error checking, no nothing */
#elif __ANDROID__
        /* FIXME: no modes, no error checking, no nothing */
#elif HAVE_STDIO_H
            case StreamType::StdIn:  m_fd = stdin;  break;
            case StreamType::StdOut: m_fd = stdout; break;
            case StreamType::StdErr: m_fd = stderr; break;
#endif
        }
    }

    void Open(String const &file, FileAccess mode)
    {
        m_type = StreamType::File;
#if __CELLOS_LV2__
        String realfile = String(SYS_APP_HOME) + '/' + file;
        CellFsErrno err = cellFsOpen(realfile.C(), CELL_FS_O_RDONLY,
                                     &m_fd, NULL, 0);
        if (err != CELL_FS_SUCCEEDED)
            m_fd = -1;
#elif __ANDROID__
        ASSERT(g_assets);
        m_asset = AAssetManager_open(g_assets, file.C(), AASSET_MODE_UNKNOWN);
#elif HAVE_STDIO_H
        /* FIXME: no modes, no error checking, no nothing */
        m_fd = fopen(file.C(), "r");
#endif
    }

    inline bool IsValid() const
    {
#if __CELLOS_LV2__
        return m_fd > -1;
#elif __ANDROID__
        return !!m_asset;
#elif HAVE_STDIO_H
        return !!m_fd;
#else
        return false;
#endif
    }

    int Read(uint8_t *buf, int count)
    {
#if __CELLOS_LV2__
        uint64_t done;
        CellFsErrno err = cellFsRead(m_fd, buf, count, &done);

        if (err != CELL_FS_SUCCEEDED)
            return -1;

        return (int)done;
#elif __ANDROID__
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
        Array<uint8_t> buf;
        buf.Resize(BUFSIZ);
        String ret;
        while (IsValid())
        {
            int done = Read(&buf[0], buf.Count());

            if (done <= 0)
                break;

            int oldsize = ret.Count();
            ret.Resize(oldsize + done);
            memcpy(&ret[oldsize], &buf[0], done);

            buf.Resize(buf.Count() * 3 / 2);
        }
        return ret;
    }

    int Write(uint8_t const *buf, int count)
    {
#if __CELLOS_LV2__
/*
        uint64_t done;
        CellFsErrno err = cellFsRead(m_fd, buf, count, &done);

        if (err != CELL_FS_SUCCEEDED)
            return -1;

        return (int)done;
*/
        return 0;
#elif __ANDROID__
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
        return Write((uint8_t const *)buf.C(), buf.Count());
    }

    void Close()
    {
        if (m_type != StreamType::File)
            return;
#if __CELLOS_LV2__
        if (m_fd >= 0)
            cellFsClose(m_fd);
        m_fd = -1;
#elif __ANDROID__
        if (m_asset)
            AAsset_close(m_asset);
        m_asset = nullptr;
#elif HAVE_STDIO_H
        if (m_fd)
            fclose(m_fd);
        m_fd = nullptr;
#endif
    }

#if __CELLOS_LV2__
    int m_fd;
#elif __ANDROID__
    AAsset *m_asset;
#elif HAVE_STDIO_H
    FILE *m_fd;
#endif
    Atomic<int> m_refcount;
    StreamType m_type;
};

File::File()
  : m_data(new FileData)
{
    ++m_data->m_refcount;
}

File::File(File const &that)
  : m_data(that.m_data)
{
    ++m_data->m_refcount;
}

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

File::~File()
{
    int refcount = --m_data->m_refcount;
    if (refcount == 0)
    {
        m_data->Close();
        delete m_data;
    }
}

void File::Open(StreamType stream)
{
    return m_data->Open(stream);
}

void File::Open(String const &file, FileAccess mode)
{
    return m_data->Open(file, mode);
}

bool File::IsValid() const
{
    return m_data->IsValid();
}

int File::Read(uint8_t *buf, int count)
{
    return m_data->Read(buf, count);
}

String File::ReadString()
{
    return m_data->ReadString();
}

int File::Write(uint8_t const *buf, int count)
{
    return m_data->Write(buf, count);
}

int File::WriteString(const String &buf)
{
    return m_data->WriteString(buf);
}

void File::Close()
{
    m_data->Close();
}

} /* namespace lol */

