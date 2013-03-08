//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#if __CELLOS_LV2__
#   include <sys/paths.h>
#   include <cell/cell_fs.h>
#endif

#include "core.h"

namespace lol
{

class FileData
{
    friend class File;

    void Open(String const &file, FileAccess mode)
    {
#if __CELLOS_LV2__
        String realfile = String(SYS_APP_HOME) + '/' + file;
        CellFsErrno err = cellFsOpen(realfile.C(), CELL_FS_O_RDONLY,
                                     &m_fd, NULL, 0);
        if (err != CELL_FS_SUCCEEDED)
            m_fd = -1;
#elif HAVE_STDIO_H
        /* FIXME: no modes, no error checking, no nothing */
        m_fd = fopen(file.C(), "r");
#endif
    }

    inline bool IsValid() const
    {
#if __CELLOS_LV2__
        return m_fd > -1;
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

    void Close()
    {
#if __CELLOS_LV2__
        if (m_fd >= 0)
            cellFsClose(m_fd);
#elif HAVE_STDIO_H
        if (m_fd)
            fclose(m_fd);
        m_fd = nullptr;
#endif
    }

#if __CELLOS_LV2__
    int m_fd;
#elif HAVE_STDIO_H
    FILE *m_fd;
#endif
    Atomic<int> m_refcount;
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

void File::Close()
{
    m_data->Close();
}

} /* namespace lol */

