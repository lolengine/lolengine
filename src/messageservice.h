//
// Lol Engine
//
// Copyright: (c) 2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Message Service class
// ----------------
//

#if !defined __MESSAGESERVICE_H__
#define __MESSAGESERVICE_H__

namespace lol
{

//Utility enum for message service
struct MessageBucket
{
    enum Value
    {
        AppIn = 0,
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

        Max
    }
    m_value;

    inline MessageBucket(Value v) : m_value(v) {}
    inline MessageBucket() : m_value(AppIn) {}
    inline operator Value() { return m_value; }
};

//Message list container with time in it
struct MessageList
{
    MessageList(time_t timestamp, const String& message)
    {
        m_timestamp = timestamp;
        m_message = message;
    }

    time_t  m_timestamp;
    String  m_message;
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

    static char const *GetName() { return "<messageservice>"; }

    //Setup/Destroy
    static void Setup();
    static void Destroy();

    //Common interactions
    static bool Send(MessageBucket id, const String& message);
    static bool Send(MessageBucket id, const char* message);
    static bool FetchFirst(MessageBucket id, String& message);
    static bool FetchFirst(MessageBucket id, String& message, time_t &timestamp);
    static bool FetchAll(MessageBucket id, String& message);
    static bool FetchAll(MessageBucket id, String& message, time_t &first_timestamp);

private:
    Array<Array<MessageList> >  m_bucket;
};

extern MessageService *g_messageservice;

} /* namespace lol */

#endif /* __MESSAGESERVICE_H__ */
