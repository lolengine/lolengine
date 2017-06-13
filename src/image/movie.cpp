//
//  Lol Engine
//
//  Copyright © 2010—2016 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if LOL_USE_FFMPEG
extern "C"
{
#   include <libavformat/avformat.h>
}
#endif

namespace lol
{

#if LOL_USE_FFMPEG
static bool g_ready = false;
#endif

class MovieData
{
#if LOL_USE_FFMPEG

#endif

    friend class Movie;
};

/*
 * Public Movie class
 */

Movie::Movie(String const &name, ivec2 size, float fps)
  : m_data(new MovieData())
{
#if LOL_USE_FFMPEG
    if (!g_ready)
    {
        g_ready = true;
        av_register_all();
    }
#endif
}

Movie::~Movie()
{
#if LOL_USE_FFMPEG

#endif

    delete m_data;
}

void Movie::Feed(image const &image)
{
#if LOL_USE_FFMPEG

#endif
}

} /* namespace lol */

