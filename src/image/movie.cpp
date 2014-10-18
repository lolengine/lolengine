//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if USE_FFMPEG
extern "C"
{
#   include <libavformat/avformat.h>
}
#endif

namespace lol
{

#if USE_FFMPEG
static bool g_ready = false;
#endif

class MovieData
{
#if USE_FFMPEG

#endif

    friend class Movie;
};

/*
 * Public Movie class
 */

Movie::Movie(String const &name, ivec2 size, float fps)
  : m_data(new MovieData())
{
#if USE_FFMPEG
    if (!g_ready)
    {
        g_ready = true;
        av_register_all();
    }
#endif
}

Movie::~Movie()
{
#if USE_FFMPEG

#endif

    delete m_data;
}

void Movie::Feed(Image const &image)
{
#if USE_FFMPEG

#endif
}

} /* namespace lol */

