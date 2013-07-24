//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// File and buffer reading
// -----------------------
//

#if !defined __LOL_SYS_FILE_H__
#define __LOL_SYS_FILE_H__

#include <stdint.h>

namespace lol
{

struct FileAccess
{
    enum Value
    {
        Read = 0,
        Write,
    }
    m_value;

    inline FileAccess(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

class File
{
public:
    File();
    File(File const &that);
    File &operator =(File const &that);
    ~File();

    void Open(String const &file, FileAccess mode);
    bool IsValid() const;
    void Close();

    int Read(uint8_t *buf, int count);
    String ReadString();

private:
    class FileData *m_data;
};

} /* namespace lol */

#endif // __LOL_SYS_FILE_H__

