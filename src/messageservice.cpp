//
//  Lol Engine
//
//  Copyright © 2013—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//            © 2017—2018 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <string>
#include <cstring>
#include <cstdlib>
#include <time.h>

namespace lol
{

// This is needed to map MessageService::Send() to the JS when building the HTML.
#if EMSCRIPTEN
extern "C"
{
    int C_Send(const char* message) { return (int)MessageService::Send(MessageBucket::AppIn, message); }
    //NOT IMPLEMENTED
    //bool C_FetchFirst(std::string& message);
}
#endif //EMSCRIPTEN

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
    m_bucket.empty();
}

//Setup/Destroy
void MessageService::Setup()
{
    g_messageservice = new MessageService();
    g_messageservice->m_bucket.resize(MessageBucket::MAX);
}

void MessageService::Destroy()
{
    delete g_messageservice;
    g_messageservice = nullptr;
}

//-----------------------------------------------------------------------------
bool MessageService::Send(MessageBucket id, const std::string& message)
{
    if (g_messageservice)
    {
        MessageService& g = *g_messageservice;
        array<MessageList>& bucket = g.m_bucket[id.ToScalar()];
        bucket << MessageList(time(nullptr), message);
        return true;
    }
    return false;
}

//----
bool MessageService::FetchFirst(MessageBucket id, std::string& message)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id.ToScalar() && id.ToScalar() < g_messageservice->m_bucket.count());
        time_t timestamp;
        return g_messageservice->FetchFirst(id, message, timestamp);
    }
    return false;
}

bool MessageService::FetchFirst(MessageBucket id, std::string& message, time_t& timestamp)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id.ToScalar() && id.ToScalar() < g_messageservice->m_bucket.count());
        MessageService& g = *g_messageservice;
        array<MessageList>& bucket = g.m_bucket[id.ToScalar()];

        if (bucket.count())
        {
            message = bucket[0].m_message;
            timestamp = bucket[0].m_timestamp;
            bucket.remove(0);
            return true;
        }
    }
    return false;
}

//----
bool MessageService::FetchAll(MessageBucket id, std::string& message)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id.ToScalar() && id.ToScalar() < g_messageservice->m_bucket.count());
        time_t timestamp;
        return g_messageservice->FetchAll(id, message, timestamp);
    }
    return false;
}

bool MessageService::FetchAll(MessageBucket id, std::string& message, time_t& first_timestamp)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id.ToScalar() && id.ToScalar() < g_messageservice->m_bucket.count());
        MessageService& g = *g_messageservice;
        array<MessageList>& bucket = g.m_bucket[id.ToScalar()];
        message = "";

        if (bucket.count())
        {
            first_timestamp = bucket[0].m_timestamp;
            for (int i = 0; i < bucket.count(); ++i)
                message += bucket[i].m_message;
            bucket.empty();
            return true;
        }
    }
    return false;
}

} /* namespace lol */

