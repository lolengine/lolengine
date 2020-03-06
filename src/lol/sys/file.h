//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
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

#include <string>   // std::string
#include <vector>   // std::vector
#include <map>      // std::map
#include <stdint.h> // int64_t

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
    virtual bool BuildEnumMap(std::map<int64_t, std::string>& enum_map)
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
    virtual bool BuildEnumMap(std::map<int64_t, std::string>& enum_map)
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
    void Open(std::string const &file, FileAccess mode, bool force_binary=false);
    bool IsValid() const;
    void Close();

    int Read(uint8_t *buf, int count);
    std::string ReadString();
    int Write(void const *buf, int count);
    int Write(std::string const &buf);
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
    Directory(std::string const &name);
    Directory(Directory const &that);
    Directory &operator =(Directory const &that);
    ~Directory();

    void Open(FileAccess mode);
    bool IsValid() const;
    void Close();

private:
    bool GetContent(std::vector<std::string>* files, std::vector<Directory>* directories);
public:
    bool GetContent(std::vector<std::string>& files, std::vector<Directory>& directories);
    bool GetContent(std::vector<Directory>& directories);
    bool GetContent(std::vector<std::string>& files);
    std::string GetName();
    long int GetModificationTime();

    static std::string GetCurrent();
    static bool SetCurrent(std::string directory);

private:
    class DirectoryData* m_data;
    std::string m_name;
};

} /* namespace lol */

