//
// Lol Engine
//
// Copyright: (c) 2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//                2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstring>
#include <cstdlib>
#include <time.h>

#include "core.h"

namespace lol
{

/*
 * The global g_renderer object, initialised by Video::Init
 */
MessageService *g_messageservice = nullptr;

//-----------------------------------------------------------------------------
// Ctor/Dtor
//----
MessageService::MessageService()
{
}

MessageService::~MessageService()
{
    m_bucket.Empty();
}

//Setup/Destroy
void MessageService::Setup(int bucket_size)
{
    g_messageservice = new MessageService();
    g_messageservice->m_bucket.Resize(bucket_size);
}

void MessageService::Destroy()
{
    delete g_messageservice;
    g_messageservice = nullptr;
}

//-----------------------------------------------------------------------------
bool MessageService::Send(int id, const String& message)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id && id < g_messageservice->m_bucket.Count());
        return g_messageservice->Send(id, message.C());
    }
    return false;
}

bool MessageService::Send(int id, const char* message)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id && id < g_messageservice->m_bucket.Count());
        MessageService& g = *g_messageservice;
        Array<MessageList>& bucket = g.m_bucket[id];
        bucket << MessageList(time(nullptr), String(message));
        return true;
    }
    return false;
}

//----
bool MessageService::FetchFirst(int id, String& message)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id && id < g_messageservice->m_bucket.Count());
        time_t timestamp;
        return g_messageservice->FetchFirst(id, message, timestamp);
    }
    return false;
}

bool MessageService::FetchFirst(int id, String& message, time_t& timestamp)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id && id < g_messageservice->m_bucket.Count());
        MessageService& g = *g_messageservice;
        Array<MessageList>& bucket = g.m_bucket[id];

        if (bucket.Count())
        {
            message = bucket[0].m_message;
            timestamp = bucket[0].m_timestamp;
            bucket.Remove(0);
            return true;
        }
    }
    return false;
}

//----
bool MessageService::FetchAll(int id, String& message)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id && id < g_messageservice->m_bucket.Count());
        time_t timestamp;
        return g_messageservice->FetchAll(id, message, timestamp);
    }
    return false;
}

bool MessageService::FetchAll(int id, String& message, time_t& first_timestamp)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id && id < g_messageservice->m_bucket.Count());
        MessageService& g = *g_messageservice;
        Array<MessageList>& bucket = g.m_bucket[id];
        message = String("");

        if (bucket.Count())
        {
            first_timestamp = bucket[0].m_timestamp;
            for (int i = 0; i < bucket.Count(); ++i)
                message += bucket[i].m_message;
            bucket.Empty();
            return true;
        }
    }
    return false;
}

} /* namespace lol */
