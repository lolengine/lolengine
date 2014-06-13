/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2004-2008 Sam Hocevar <sam@zoy.org>
 *                2008 Jean-Yves Lamoureux <jylam@lnxscene.org>
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
 * jpeg.c: libjpeg I/O functions
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>

#include <jpeglib.h>

#include "pipi.h"
#include "pipi-internals.h"

static int pipi_free_jpeg(pipi_image_t *img);

struct my_error_mgr
{
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

static void format_msg(j_common_ptr cinfo, char *buf)
{
}

static void emit_msg(j_common_ptr cinfo, int level)
{
}

static void error_msg(j_common_ptr cinfo)
{
    my_error_ptr myerr = (my_error_ptr) cinfo->err;
    cinfo->client_data = (void*)0x1;
    longjmp(myerr->setjmp_buffer, 1);
}

static void output_msg(j_common_ptr cinfo)
{
}

pipi_image_t *pipi_load_jpeg(const char *name)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;
    unsigned char *image = NULL, *scanline = NULL;
    pipi_image_t *img = NULL;
    unsigned int i, j, k = 0;
    FILE *fp;

    fp = fopen(name, "rb");
    if(!fp)
        return NULL;

    if(setjmp(jerr.setjmp_buffer))
        goto end;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = error_msg;
    jerr.pub.emit_message = emit_msg;
    jerr.pub.output_message = output_msg;
    jerr.pub.format_message = format_msg;

    /* Initialize libjpeg */
    jpeg_create_decompress(&cinfo);
    cinfo.client_data = 0x0;
    jpeg_stdio_src(&cinfo, fp);
    if(cinfo.client_data == (void *)0x1)
        goto end;
    jpeg_read_header(&cinfo, TRUE);
    if(cinfo.client_data == (void *)0x1)
        goto end;
    jpeg_start_decompress(&cinfo);
    if(cinfo.client_data == (void *)0x1)
        goto end;

    /* One scanline */
    image = malloc(cinfo.output_width * cinfo.output_height * 4);
    if(!image)
        goto end;

    scanline = malloc(cinfo.output_width * 3);

    for(i = 0; i < cinfo.output_height; i++)
    {
        jpeg_read_scanlines(&cinfo, &scanline, 1);
        if(cinfo.client_data == (void *)0x1)
        {
            free(img);
            img = NULL;
            goto end;
        }
        for(j = 0; j < cinfo.output_width * 3; j += 3)
        {
            image[k + 2] = scanline[j];
            image[k + 1] = scanline[j + 1];
            image[k]     = scanline[j + 2];
            image[k + 3] = 255;
            k += 4;
        }
    }

    img = pipi_new(cinfo.output_width, cinfo.output_height);

    img->p[PIPI_PIXELS_RGBA_U8].pixels = image;
    img->p[PIPI_PIXELS_RGBA_U8].w = cinfo.output_width;
    img->p[PIPI_PIXELS_RGBA_U8].h = cinfo.output_height;
    img->p[PIPI_PIXELS_RGBA_U8].pitch = cinfo.output_width * 4;
    img->p[PIPI_PIXELS_RGBA_U8].bpp = 24;
    img->p[PIPI_PIXELS_RGBA_U8].bytes = 4 * img->w * img->h;
    img->last_modified = PIPI_PIXELS_RGBA_U8;

    img->codec_priv = (void *)&cinfo;
    img->codec_format = PIPI_PIXELS_RGBA_U8;
    img->codec_free = pipi_free_jpeg;

    img->wrap = 0;
    img->u8 = 1;

end:
    if(fp)
        fclose(fp);
    if(scanline)
        free(scanline);
    jpeg_destroy_decompress(&cinfo);
    return img;
}

int pipi_save_jpeg(pipi_image_t *img, const char *name)
{
    struct jpeg_compress_struct cinfo;
    struct my_error_mgr jerr;
    unsigned char *data;
    unsigned char *line;
    pipi_pixels_t *pixels;
    JSAMPROW *jbuf;
    uint32_t *ptr;
    FILE *fp;
    int i, j, y = 0;
    size_t len;

    int quality = 75; /* FIXME */

    len = strlen(name);
    if(len < 4 || name[len - 4] != '.'
        || toupper(name[len - 3]) != 'J'
        || toupper(name[len - 2]) != 'P'
        || toupper(name[len - 1]) != 'G')
    {
        if(len < 5 || name[len - 5] != '.'
            || toupper(name[len - 4]) != 'J'
            || toupper(name[len - 3]) != 'P'
            || toupper(name[len - 2]) != 'E'
            || toupper(name[len - 1]) != 'G')
            return -1;
    }

    pixels = pipi_get_pixels(img, PIPI_PIXELS_RGBA_U8);
    if(!pixels)
        return -1;

    data = pixels->pixels;

    line = malloc(img->w * 3 * sizeof(unsigned char));
    if(!line)
        return -1;

    fp = fopen(name, "wb");
    if(!fp)
    {
        free(line);
        return -1;
    }

    if(setjmp(jerr.setjmp_buffer))
        goto end;

    jerr.pub.error_exit = error_msg;
    jerr.pub.emit_message = emit_msg;
    jerr.pub.output_message = output_msg;

    cinfo.err = jpeg_std_error(&(jerr.pub));

    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, fp);
    cinfo.image_width = img->w;
    cinfo.image_height = img->h;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    ptr = (uint32_t*)data;

    while(cinfo.next_scanline < cinfo.image_height)
    {
        for (j = 0, i = 0; i < img->w; i++)
        {
            line[j++] = ((*ptr) >> 16) & 0xff;
            line[j++] = ((*ptr) >> 8) & 0xff;
            line[j++] = ((*ptr)) & 0xff;
            ptr++;
        }
        jbuf = (JSAMPROW *) (&line);
        jpeg_write_scanlines(&cinfo, jbuf, 1);
        y++;
    }

    jpeg_finish_compress(&cinfo);

end:
    jpeg_destroy_compress(&cinfo);
    free(line);
    fclose(fp);

    pipi_release_pixels(img, pixels);

    return 0;
}

static int pipi_free_jpeg(pipi_image_t *img)
{
    if(img->p[PIPI_PIXELS_RGBA_U8].pixels)
        free(img->p[PIPI_PIXELS_RGBA_U8].pixels);
    return 0;
}

