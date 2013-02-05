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

#include "core.h"

namespace lol
{

class FileData
{
    friend class File;

    void Open(String const &file, FileAccess mode)
    {
#if __CELLOS_LV2__
#elif HAVE_STDIO_H
        /* FIXME: no modes, no error checking, no nothing */
        m_fd = fopen(file.C(), "r");
#endif
    }

    String ReadString()
    {
        String ret;
#if __CELLOS_LV2__
#elif HAVE_STDIO_H
        while (!feof(m_fd))
        {
            char buf[BUFSIZ];
            size_t count = fread(buf, 1, BUFSIZ, m_fd);
            if (count <= 0)
                break;

            int oldsize = ret.Count();
            ret.Resize(oldsize + count);
            memcpy(&ret[oldsize], buf, count);
        }
#endif
        return ret;
    }

    void Close()
    {
#if __CELLOS_LV2__
#elif HAVE_STDIO_H
        fclose(m_fd);
#endif
    }

#if __CELLOS_LV2__
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

String File::ReadString()
{
    return m_data->ReadString();
}

void File::Close()
{
    m_data->Close();
}

} /* namespace lol */

