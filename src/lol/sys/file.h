//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// File and buffer reading
// -----------------------
//

#include <map>
#include <cstdint>

namespace lol
{

//FileAccessBase --------------------------------------------------------------
struct FileAccessBase : public StructSafeEnum
{
    enum Type
    {
        Read = 0,
        Write
    };
protected:
    virtual bool BuildEnumMap(std::map<int64_t, String>& enum_map)
    {
        enum_map[Read]  = "Read";
        enum_map[Write] = "Write";
        return true;
    }
};
typedef SafeEnum<FileAccessBase> FileAccess;

//StreamTypeBase --------------------------------------------------------------
struct StreamTypeBase : public StructSafeEnum
{
    enum Type
    {
        StdIn,
        StdOut,
        StdErr,
        File,
        FileBinary
    };
protected:
    virtual bool BuildEnumMap(std::map<int64_t, String>& enum_map)
    {
        enum_map[StdIn]      = "StdIn";
        enum_map[StdOut]     = "StdOut";
        enum_map[StdErr]     = "StdErr";
        enum_map[File]       = "File";
        enum_map[FileBinary] = "FileBinary";
        return true;
    }
};
typedef SafeEnum<StreamTypeBase> StreamType;

class File
{
public:
    File();
    File(File const &directory);
    File &operator =(File const &that);
    ~File();

    void Open(StreamType stream);
    void Open(String const &file, FileAccess mode, bool force_binary=false);
    bool IsValid() const;
    void Close();

    int Read(uint8_t *buf, int count);
    String ReadString();
    int Write(uint8_t const *buf, int count);
    int WriteString(const String &buf);
    long int GetPosFromStart();
    void SetPosFromStart(long int pos);
    long int size();
    long int GetModificationTime();

private:
    class FileData *m_data;
};

class Directory
{
public:
    Directory(String const &name);
    Directory(Directory const &that);
    Directory &operator =(Directory const &that);
    ~Directory();

    void Open(FileAccess mode);
    bool IsValid() const;
    void Close();

private:
    bool GetContent(array<String>* files, array<Directory>* directories);
public:
    bool GetContent(array<String>& files, array<Directory>& directories);
    bool GetContent(array<Directory>& directories);
    bool GetContent(array<String>& files);
    String GetName();
    long int GetModificationTime();

    static String GetCurrent();
    static bool SetCurrent(String directory);

private:
    class DirectoryData*    m_data;
    String                  m_name;
};

} /* namespace lol */

