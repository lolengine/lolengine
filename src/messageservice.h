//
//  Lol Engine
//
//  Copyright © 2017—2020 Sam Hocevar <sam@hocevar.net>
//            © 2013—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

#include <string>
#include <map>

//
// The Message Service class
// -------------------------
//

namespace lol
{

//MessageBucket -- Utility enum for message service ---------------------------
struct MessageBucketBase : public StructSafeEnum
{
    enum Type
    {
        AppIn,
        AppOut,

        Bckt0,
        Bckt1,
        Bckt2,
        Bckt3,
        Bckt4,
        Bckt5,
        Bckt6,
        Bckt7,
        Bckt8,
        Bckt9,

        MAX
    };
protected:
    virtual bool BuildEnumMap(std::map<int64_t, std::string>& enum_map)
    {
        enum_map[AppIn] = "AppIn";
        enum_map[AppOut] = "AppOut";
        enum_map[Bckt0] = "Bckt0";
        enum_map[Bckt1] = "Bckt1";
        enum_map[Bckt2] = "Bckt2";
        enum_map[Bckt3] = "Bckt3";
        enum_map[Bckt4] = "Bckt4";
        enum_map[Bckt5] = "Bckt5";
        enum_map[Bckt6] = "Bckt6";
        enum_map[Bckt7] = "Bckt7";
        enum_map[Bckt8] = "Bckt8";
        enum_map[Bckt9] = "Bckt9";
        enum_map[MAX] = "MAX";
        return true;
    }
};
typedef SafeEnum<MessageBucketBase> MessageBucket;

//Message list container with time in it
struct MessageList
{
    MessageList(time_t timestamp, const std::string& message)
      : m_timestamp(timestamp),
        m_message(message)
    {
    }

    time_t m_timestamp;
    std::string m_message;
};

/*
    A template class perhaps ?
*/
class MessageService
{
public:
    //CTor/DTor
    MessageService();
    ~MessageService();

    static std::string GetName() { return "<messageservice>"; }

    //Setup/Destroy
    static void Setup();
    static void Destroy();

    //Common interactions
    static bool Send(MessageBucket id, const std::string& message);
    static bool FetchFirst(MessageBucket id, std::string& message);
    static bool FetchFirst(MessageBucket id, std::string& message, time_t &timestamp);
    static bool FetchAll(MessageBucket id, std::string& message);
    static bool FetchAll(MessageBucket id, std::string& message, time_t &first_timestamp);

private:
    std::vector<std::vector<MessageList> >  m_bucket;
};

extern MessageService *g_messageservice;

} /* namespace lol */

