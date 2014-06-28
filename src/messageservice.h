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

// Utility enum for message service
struct MessageBucket
{
    DEF_VALUE
        ADD_VALUE(AppIn)
        ADD_VALUE(AppOut)

        ADD_VALUE(Bckt0)
        ADD_VALUE(Bckt1)
        ADD_VALUE(Bckt2)
        ADD_VALUE(Bckt3)
        ADD_VALUE(Bckt4)
        ADD_VALUE(Bckt5)
        ADD_VALUE(Bckt6)
        ADD_VALUE(Bckt7)
        ADD_VALUE(Bckt8)
        ADD_VALUE(Bckt9)
    END_E_VALUE

    LOL_DECLARE_ENUM_METHODS(MessageBucket)
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
    array<array<MessageList> >  m_bucket;
};

extern MessageService *g_messageservice;

} /* namespace lol */

#endif /* __MESSAGESERVICE_H__ */
