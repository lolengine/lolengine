//
//  Lol Engine
//
//  Copyright © 2010—2017 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The movie class
// ---------------
//

#include <lol/image/pixel.h>
#include <lol/image/image.h>

extern "C" struct AVFormatContext;
extern "C" struct AVCodecContext;
extern "C" struct AVStream;
extern "C" struct AVFrame;

namespace lol
{

class movie
{
public:
    movie(ivec2 size);

    bool open_file(char const *filename);
    bool push_image(image &im);
    void close();

private:
    bool open_codec();

private:
    AVFormatContext *m_avformat;
    AVCodecContext *m_avcodec;
    AVStream *m_stream;
    AVFrame *m_frame;
    ivec2 m_size;
    int m_index;
};

} // namespace lol

