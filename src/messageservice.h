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
    static void Setup(int bucket_size);
    static void Destroy();

    //Common interactions
    static bool Send(int id, const String& message);
    static bool Send(int id, const char* message);
    static bool FetchFirst(int id, String& message);
    static bool FetchFirst(int id, String& message, time_t &timestamp);
    static bool FetchAll(int id, String& message);
    static bool FetchAll(int id, String& message, time_t &first_timestamp);

private:
    Array<Array<MessageList> >  m_bucket;
};

extern MessageService *g_messageservice;

} /* namespace lol */

#endif /* __MESSAGESERVICE_H__ */
