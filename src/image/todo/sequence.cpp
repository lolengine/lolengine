/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2004-2009 Sam Hocevar <sam@hocevar.net>
 *                All Rights Reserved
 *
 *  $Id$
 *
 *  This library is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What The Fuck You Want
 *  To Public License, Version 2, as published by Sam Hocevar. See
 *  http://sam.zoy.org/wtfpl/COPYING for more details.
 */

/*
 * codec.c: image I/O functions
 */

#include "config.h"

#if defined _WIN32
#   undef _CRT_SECURE_NO_WARNINGS
#   define _CRT_SECURE_NO_WARNINGS /* I know how to use snprintf, thank you */
#   define snprintf _snprintf
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined USE_FFMPEG
#   include <libavformat/avformat.h>
#   include <libswscale/swscale.h>
#endif

#include "pipi.h"
#include "pipi-internals.h"

#if defined USE_FFMPEG
typedef struct
{
    uint8_t *buf;
    size_t buf_len;

    AVFormatContext *fmt_ctx;
    AVStream *stream;
    AVCodecContext *cod_ctx;
    AVCodec *codec;
    AVFrame *frame;

    struct SwsContext *sws_ctx;
    int src_width, src_height, src_fmt;
}
ffmpeg_codec_t;
#endif

pipi_sequence_t *pipi_open_sequence(char const *file,
                                    int width, int height, int rgb, int fps,
                                    int par_num, int par_den, int bitrate)
{
#if defined USE_FFMPEG
    static int initialised = 0;

    pipi_sequence_t *seq;
    ffmpeg_codec_t *ff;
    uint8_t *tmp;

    seq = malloc(sizeof(pipi_sequence_t));
    seq->w = width;
    seq->h = height;
    seq->fps = fps;
    seq->convert_buf = NULL;

    ff = malloc(sizeof(ffmpeg_codec_t));
    memset(ff, 0, sizeof(*ff));

    seq->codec_priv = ff;

    if (!initialised)
    {
        av_register_all();
        initialised = 1;
    }

    ff->fmt_ctx = avformat_alloc_context();
    if (!ff->fmt_ctx)
        goto error;

    /* Careful here: the Win32 snprintf doesn't seem to add a trailing
     * zero to the truncated output. */
    snprintf(ff->fmt_ctx->filename, sizeof(ff->fmt_ctx->filename),
             file);
    ff->fmt_ctx->filename[sizeof(ff->fmt_ctx->filename) - 1] = '\0';

    ff->fmt_ctx->oformat = av_guess_format(NULL, file, NULL);
    if (!ff->fmt_ctx->oformat)
        ff->fmt_ctx->oformat = av_guess_format("mpeg", NULL, NULL);
    if (!ff->fmt_ctx->oformat)
        goto error;

    ff->stream = av_new_stream(ff->fmt_ctx, 0);
    if (!ff->stream)
        goto error;

    ff->stream->sample_aspect_ratio.num = par_num;
    ff->stream->sample_aspect_ratio.den = par_den;

    ff->cod_ctx = ff->stream->codec;

    ff->cod_ctx->width = width;
    ff->cod_ctx->height = height;
    ff->cod_ctx->sample_aspect_ratio.num = par_num;
    ff->cod_ctx->sample_aspect_ratio.den = par_den;
    ff->cod_ctx->codec_id = ff->fmt_ctx->oformat->video_codec;
    ff->cod_ctx->codec_type = CODEC_TYPE_VIDEO;
    ff->cod_ctx->bit_rate = bitrate;
    ff->cod_ctx->time_base.num = 1;
    ff->cod_ctx->time_base.den = fps;

    ff->cod_ctx->pix_fmt = PIX_FMT_YUV420P; /* send YUV 420 */
    if (ff->cod_ctx->codec_id == CODEC_ID_MPEG2VIDEO)
        ff->cod_ctx->max_b_frames = 2;
    if (ff->cod_ctx->codec_id == CODEC_ID_MPEG1VIDEO)
        ff->cod_ctx->mb_decision = 2;
    if (ff->cod_ctx->codec_id == CODEC_ID_H264)
    {
        /* Import x264 slow presets */
        ff->cod_ctx->coder_type = 1;
        ff->cod_ctx->flags |= CODEC_FLAG_LOOP_FILTER;
        ff->cod_ctx->me_cmp |= FF_CMP_CHROMA;
        ff->cod_ctx->partitions |= X264_PART_I4X4 | X264_PART_I8X8
                                 | X264_PART_P4X4 | X264_PART_P8X8;
        ff->cod_ctx->me_method = ME_UMH;
        ff->cod_ctx->me_subpel_quality = 8;
        ff->cod_ctx->me_range = 16;
        ff->cod_ctx->gop_size = 250;
        ff->cod_ctx->keyint_min = 25;
        ff->cod_ctx->scenechange_threshold = 40;
        ff->cod_ctx->i_quant_factor = 0.71f;
        ff->cod_ctx->b_frame_strategy = 2;
        ff->cod_ctx->qcompress = 0.6f;
        ff->cod_ctx->qmin = 10;
        ff->cod_ctx->qmax = 51;
        ff->cod_ctx->max_qdiff = 4;
        ff->cod_ctx->max_b_frames = 3;
        ff->cod_ctx->refs = 5;
        ff->cod_ctx->directpred = 3;
        ff->cod_ctx->trellis = 1;
        ff->cod_ctx->flags2 |= CODEC_FLAG2_BPYRAMID | CODEC_FLAG2_MIXED_REFS
                             | CODEC_FLAG2_WPRED | CODEC_FLAG2_8X8DCT
                             | CODEC_FLAG2_FASTPSKIP;
        ff->cod_ctx->weighted_p_pred = 2;
        ff->cod_ctx->rc_lookahead = 50;
    }
    if (ff->fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
        ff->cod_ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;

    if (av_set_parameters(ff->fmt_ctx, NULL) < 0)
        goto error;

    ff->codec = avcodec_find_encoder(ff->cod_ctx->codec_id);
    if (!ff->codec)
        goto error;
    if (avcodec_open(ff->cod_ctx, ff->codec) < 0)
        goto error;

    ff->frame = avcodec_alloc_frame();
    if (!ff->frame)
        goto error;
    tmp = (uint8_t *)av_malloc(avpicture_get_size(ff->cod_ctx->pix_fmt,
                                                  ff->cod_ctx->width,
                                                  ff->cod_ctx->height));
    if (!tmp)
        goto error;
    avpicture_fill((AVPicture *)ff->frame, tmp, ff->cod_ctx->pix_fmt,
                   ff->cod_ctx->width, ff->cod_ctx->height);

    if (!(ff->fmt_ctx->flags & AVFMT_NOFILE))
        if (url_fopen(&ff->fmt_ctx->pb, file, URL_WRONLY) < 0)
            goto error;

    ff->buf_len = 64 * 1024 * 1024;
    ff->buf = (uint8_t *)av_malloc(ff->buf_len);

    ff->src_fmt = rgb ? PIX_FMT_RGB32 : PIX_FMT_YUV444P;

    av_write_header(ff->fmt_ctx);

    return seq;

error:
    pipi_close_sequence(seq);
    return NULL;

#else
    return NULL;

#endif
}

int pipi_feed_sequence(pipi_sequence_t *seq, uint8_t const *buffer,
                       int width, int height)
{
#if defined USE_FFMPEG
    AVPacket packet;
    uint8_t const *buflist[3];
    int pitchlist[3];
    size_t bytes;
    int n;

    ffmpeg_codec_t *ff = (ffmpeg_codec_t *)seq->codec_priv;

    if (ff->src_width != width || ff->src_height != height)
    {
        ff->src_width = width;
        ff->src_height = height;
        if (ff->sws_ctx)
            sws_freeContext(ff->sws_ctx);
        ff->sws_ctx = NULL;
    }

    if (!ff->sws_ctx)
    {
        ff->sws_ctx = sws_getContext(width, height, ff->src_fmt,
                                     ff->cod_ctx->width,
                                     ff->cod_ctx->height,
                                     ff->cod_ctx->pix_fmt, SWS_BICUBIC,
                                     NULL, NULL, NULL);
        if (seq->convert_buf)
        {
           free(seq->convert_buf);
           seq->convert_buf = NULL;
        }
    }
    if (!ff->sws_ctx)
        return -1;

    /* Convert interleaved YUV to planar YUV */
    if (ff->src_fmt == PIX_FMT_YUV444P)
    {
        if (!seq->convert_buf)
            seq->convert_buf = malloc(width * height * 3);

        for (n = 0; n < width * height; n++)
        {
            seq->convert_buf[n] = buffer[4 * n];
            seq->convert_buf[n + width * height] = buffer[4 * n + 1];
            seq->convert_buf[n + 2 * width * height] = buffer[4 * n + 2];
        }

        /* Feed the buffers to FFmpeg */
        buflist[0] = seq->convert_buf;
        buflist[1] = seq->convert_buf + 2 * width * height;
        buflist[2] = seq->convert_buf + width * height;
        pitchlist[0] = pitchlist[1] = pitchlist[2] = width;
    }
    else
    {
        buflist[0] = buffer;
        pitchlist[0] = 4 * width;
    }

    sws_scale(ff->sws_ctx, buflist, pitchlist, 0, height,
              ff->frame->data, ff->frame->linesize);

    bytes = avcodec_encode_video(ff->cod_ctx, ff->buf,
                                 ff->buf_len, ff->frame);
    if (bytes <= 0)
        return 0;

    av_init_packet(&packet);
    if (ff->cod_ctx->coded_frame->pts != 0x8000000000000000LL)
        packet.pts = av_rescale_q(ff->cod_ctx->coded_frame->pts,
                                  ff->cod_ctx->time_base, ff->stream->time_base);
    if (ff->cod_ctx->coded_frame->key_frame)
        packet.flags |= PKT_FLAG_KEY;
    packet.stream_index = 0;
    packet.data = ff->buf;
    packet.size = bytes;

    if (av_interleaved_write_frame(ff->fmt_ctx, &packet) < 0)
        return -1;
#endif

    return 0;
}

int pipi_close_sequence(pipi_sequence_t *seq)
{
#if defined USE_FFMPEG
    ffmpeg_codec_t *ff = (ffmpeg_codec_t *)seq->codec_priv;

    /* Finish the sequence */
    if (ff->buf)
    {
        av_write_trailer(ff->fmt_ctx);
    }

    /* Close everything */
    if (ff->buf)
    {
        av_free(ff->buf);
        ff->buf = NULL;
    }

    if (ff->cod_ctx)
    {
        avcodec_close(ff->cod_ctx);
        ff->cod_ctx = NULL;
    }

    if (ff->frame)
    {
        av_free(ff->frame->data[0]);
        av_free(ff->frame);
        ff->frame = NULL;
    }

    if (ff->fmt_ctx)
    {
        av_freep(&ff->fmt_ctx->streams[0]->codec);
        ff->codec = NULL;

        av_freep(&ff->fmt_ctx->streams[0]);
        ff->stream = NULL;

        if (!(ff->fmt_ctx->flags & AVFMT_NOFILE) && ff->fmt_ctx->pb)
            url_fclose(ff->fmt_ctx->pb);

        av_free(ff->fmt_ctx);
        ff->fmt_ctx = NULL;
    }

    if (ff->sws_ctx)
    {
        sws_freeContext(ff->sws_ctx);
        ff->sws_ctx = NULL;
        ff->src_width = ff->src_height = 0;
    }

    free(ff);
    free(seq);
#endif

    return 0;
}
