//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
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
#   include <libavutil/avassert.h>
#   include <libavutil/channel_layout.h>
#   include <libavutil/mathematics.h>
#   include <libavutil/timestamp.h>
#   include <libavformat/avformat.h>
#   include <libswscale/swscale.h>
#   include <libswresample/swresample.h>
}
#endif

namespace lol
{

#if LOL_USE_FFMPEG
#define ERROR_TO_STRING(errnum) (error2string((errnum)).c_str())
static std::string error2string(int errnum)
{
    char tmp[AV_ERROR_MAX_STRING_SIZE];
    av_strerror(errnum, tmp, AV_ERROR_MAX_STRING_SIZE);
    return std::string(tmp);
}

/*static void ffmpeg_logger(void *ptr, int level, const char *fmt, va_list vl)
{
    // FIXME: use lol::msg::debug
    UNUSED(ptr, level);
    vfprintf(stderr, fmt, vl);
}*/
#endif

movie::movie(ivec2 size)
  : m_avformat(nullptr),
    m_avcodec(nullptr),
    m_stream(nullptr),
    m_frame(nullptr),
    m_size(size),
    m_index(0)
{
#if LOL_USE_FFMPEG
    m_frame = av_frame_alloc();
    ASSERT(m_frame);

    m_frame->format = AV_PIX_FMT_RGB8; // 3:3:2 packed for GIF
    m_frame->width = m_size.x;
    m_frame->height = m_size.y;

    int ret = av_frame_get_buffer(m_frame, 32);
    ASSERT(ret >= 0);
#endif
}

bool movie::open_file(std::string const &filename)
{
#if LOL_USE_FFMPEG
    /* Third argument specifies format */
    avformat_alloc_output_context2(&m_avformat, nullptr, "gif", filename.c_str());
    if (!m_avformat)
    {
        msg::debug("could not create output context");
        return false;
    }

    if (!open_codec())
        return false;

    if (!(m_avformat->oformat->flags & AVFMT_NOFILE))
    {
        int ret = avio_open(&m_avformat->pb, filename.c_str(), AVIO_FLAG_WRITE);
        if (ret < 0)
        {
            msg::error("could not open '%s': %s\n", filename.c_str(), ERROR_TO_STRING(ret));
            return false;
        }
    }

    int ret = avformat_write_header(m_avformat, nullptr);
    if (ret < 0)
    {
        msg::error("could not write header: %s\n", ERROR_TO_STRING(ret));
        return false;
    }
    return true;
#else
    UNUSED(filename);
    return false;
#endif
}

bool movie::push_image(image &im)
{
#if LOL_USE_FFMPEG
    // Make sure the encoder does not hold a reference on our
    // frame (GIF does that in order to compress using deltas).
    if (av_frame_make_writable(m_frame) < 0)
        return false;

    // Convert image to 3:3:2. TODO: add some dithering
    u8vec3 *data = im.lock<PixelFormat::RGB_8>();
    for (int n = 0; n < im.size().x * im.size().y; ++n)
        m_frame->data[0][n] = (data[n].r & 0xe0) | ((data[n].g & 0xe0) >> 3) | (data[n].b >> 6);
    im.unlock(data);

    m_frame->pts = m_index++;

    int ret = avcodec_send_frame(m_avcodec, m_frame);
    if (ret < 0)
    {
        msg::error("cannot send video frame: %s\n", ERROR_TO_STRING(ret));
        return false;
    }

    while (ret >= 0)
    {
        AVPacket pkt;
        memset(&pkt, 0, sizeof(pkt));
        av_init_packet(&pkt);

        ret = avcodec_receive_packet(m_avcodec, &pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return true;
        if (ret < 0)
        {
            msg::error("cannot retrieve video packet: %s\n", ERROR_TO_STRING(ret));
            return false;
        }

        pkt.stream_index = m_stream->index;
        ret = av_interleaved_write_frame(m_avformat, &pkt);
        if (ret < 0)
        {
            msg::error("cannot write video frame: %s\n", ERROR_TO_STRING(ret));
            return false;
        }
    }
#else
    UNUSED(im);
#endif

    return true;
}

void movie::close()
{
#if LOL_USE_FFMPEG
    // this must be done before m_avcodec is freed
    av_write_trailer(m_avformat);

    avcodec_free_context(&m_avcodec);
    av_frame_free(&m_frame);

    if (!(m_avformat->oformat->flags & AVFMT_NOFILE))
        avio_closep(&m_avformat->pb);

    avformat_free_context(m_avformat);
#endif
}

bool movie::open_codec()
{
#if LOL_USE_FFMPEG
    AVCodec *codec = avcodec_find_encoder(m_avformat->oformat->video_codec);
    if (!codec)
    {
        msg::error("no encoder found for %s\n", avcodec_get_name(m_avformat->oformat->video_codec));
        return false;
    }

    m_stream = avformat_new_stream(m_avformat, nullptr);
    if (!m_stream)
    {
        msg::error("cannot allocate stream\n");
        return false;
    }
    m_stream->id = 0; // first (and only?) stream
    m_stream->time_base = AVRational{ 1, 30 }; // 30 fps

    m_avcodec = avcodec_alloc_context3(codec);
    if (!m_avcodec)
    {
        msg::error("cannot allocate encoding context\n");
        return false;
    }

    m_avcodec->codec_id = m_avformat->oformat->video_codec;
    m_avcodec->width = m_frame->width;
    m_avcodec->height = m_frame->height;
    m_avcodec->pix_fmt = AVPixelFormat(m_frame->format);
    m_avcodec->time_base = m_stream->time_base;

    if (m_avformat->oformat->flags & AVFMT_GLOBALHEADER)
        m_avcodec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    int ret = avcodec_open2(m_avcodec, codec, nullptr);
    if (ret < 0)
    {
        msg::error("cannot open video codec: %s\n", ERROR_TO_STRING(ret));
        return false;
    }

    ret = avcodec_parameters_from_context(m_stream->codecpar, m_avcodec);
    if (ret < 0)
    {
        msg::error("cannot copy stream parameters\n");
        return false;
    }
#endif

    return true;
}

} // namespace lol

