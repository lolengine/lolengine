//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//            © 2016—2017 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The ResourceCodecData class
// ------------------------
//

namespace lol
{

    class ResourceCodec
    {
    public:
        virtual std::string GetName() { return "<ResourceCodec>"; }
        virtual ResourceCodecData* Load(std::string const &path) = 0;
        virtual bool Save(std::string const &path, ResourceCodecData* data) = 0;

        /* TODO: this should become more fine-grained */
        int m_priority;
    };

#define REGISTER_IMAGE_CODEC(name) \
    extern ResourceCodec *Register##name(); \
    { \
        /* Insert image codecs in a sorted list */ \
        ResourceCodec *codec = Register##name(); \
        int i = 0, prio = codec->m_priority; \
        for ( ; i < codeclist.count(); ++i) \
        { \
            if (codeclist[i]->m_priority <= prio) \
                break; \
        } \
        codeclist.insert(codec, i); \
    }

#define DECLARE_IMAGE_CODEC(name, priority) \
    ResourceCodec *Register##name() \
    { \
        ResourceCodec *ret = new name(); \
        ret->m_priority = priority; \
        return ret; \
    }

} /* namespace lol */

