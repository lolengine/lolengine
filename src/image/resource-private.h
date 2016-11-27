//
// Lol Engine
//
//  Copyright: (c) 2016—2017 Sam Hocevar <sam@hocevar.net>
//  Copyright © 2016—2017 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
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
        virtual char const *GetName() { return "<ResourceCodec>"; }
        virtual ResourceCodecData* Load(char const *path) = 0;
        virtual bool Save(char const *path, ResourceCodecData* data) = 0;

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

