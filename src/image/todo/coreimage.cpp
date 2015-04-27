/*
 *  Lol Engine
 *
 *  Copyright © 2004—2008 Sam Hocevar <sam@hocevar.net>
 *            © 2008 Jean-Yves Lamoureux <jylam@lnxscene.org>
 *
 *  This library is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What the Fuck You Want
 *  to Public License, Version 2, as published by the WTFPL Task Force.
 *  See http://www.wtfpl.net/ for more details.
 */

/*
 * coreimage.m: CoreImage (OSX) I/O functions
 */

#import "coreimage.h"

#ifdef USE_COCOA

static int pipi_free_coreimage(pipi_image_t *img);

pipi_image_t *pipi_load_coreimage(const char *name)
{
    NSAutoreleasePool *autoreleasepool = [[NSAutoreleasePool alloc] init];
    NSString *n = [NSString stringWithCString: name];
    CIImage *source;
    NSURL *url = [NSURL fileURLWithPath:n];

    source = [CIImage imageWithContentsOfURL:url];

    if(source == NULL) return NULL;

    CGRect extent = [source extent];
    size_t w = (size_t)extent.size.width;
    size_t h = (size_t)extent.size.height;


    NSBitmapImageRep * myImage;
    myImage = [[NSBitmapImageRep alloc] initWithCIImage:source];

    pipi_image_t *img;
    img = pipi_new(w, h);

    img->p[PIPI_PIXELS_RGBA_U8].w      = w;
    img->p[PIPI_PIXELS_RGBA_U8].h      = h;
    img->p[PIPI_PIXELS_RGBA_U8].pitch  = ([myImage bytesPerRow]/8) * img->w;
    img->p[PIPI_PIXELS_RGBA_U8].bpp    = [myImage bitsPerPixel];
    img->p[PIPI_PIXELS_RGBA_U8].bytes  = ([myImage bitsPerPixel]/8) * img->w * img->h;
    img->last_modified                = PIPI_PIXELS_RGBA_U8;


    /* CoreImage feeds us with BGRA while we need RGBA, so convert it.
     * We also need to get a pitch==(w*bpp) in order to pipi to opper properly.
     */

    int pitch = (img->p[PIPI_PIXELS_RGBA_U8].bpp/8);
    unsigned char *tmp  = (unsigned char*)malloc(h*w*pitch);
    unsigned char *orig = (unsigned char*)[myImage bitmapData];
    int x, y, k=0, o=0, a=[myImage bytesPerRow] - (w*([myImage bitsPerPixel]/8));

    for(y=0; y<h; y++)
    {
        for(x=0; x<w*pitch; x+=4)
        {
            if(!([myImage bitmapFormat] & NSAlphaFirstBitmapFormat))
            {
                tmp[k+2] = orig[o];
                tmp[k+1] = orig[o+1];
                tmp[k+0] = orig[o+2];
                tmp[k+3] = orig[o+3];
            } else
            {
                tmp[k+0] = orig[o];
                tmp[k+1] = orig[o+1];
                tmp[k+2] = orig[o+2];
                tmp[k+3] = orig[o+3];
            }

            o+=4;
            k+=4;
        }
        o+=a;
    }
    img->p[PIPI_PIXELS_RGBA_U8].pixels = tmp;
    img->p[PIPI_PIXELS_RGBA_U8].pitch  = w*([myImage bitsPerPixel]/8);




    img->codec_priv = (struct pipi_codec_coreimage *) malloc(sizeof(struct pipi_codec_coreimage *));
    struct pipi_codec_coreimage *infos = (struct pipi_codec_coreimage *) img->codec_priv;
    infos->format = [myImage bitmapFormat];

    pipi_pixels_t *p = pipi_get_pixels(img, PIPI_PIXELS_RGBA_U8);

    img->codec_free = pipi_free_coreimage;

    [autoreleasepool release];
    return img;
}


int pipi_save_coreimage(pipi_image_t *img, const char *name)
{
    NSAutoreleasePool *autoreleasepool = [[NSAutoreleasePool alloc] init];
    pipi_pixels_t *p = pipi_get_pixels(img, PIPI_PIXELS_RGBA_U8);

    int i;
    char *data = p->pixels;
    for(i = 0; i < img->w*img->h; i++)
    {
        unsigned char r = data[i*4 + 0];
        unsigned char g = data[i*4 + 1];
        unsigned char b = data[i*4 + 2];
        unsigned char a = data[i*4 + 3];

        /* R */    data[i*4 + 0] = b;
        /* G */    data[i*4 + 1] = g;
        /* B */    data[i*4 + 2] = r;
        /* A */    data[i*4 + 3] = a;
    }


    NSString *n = [NSString stringWithCString: name];
    NSBitmapImageRep *bitmap = [[NSBitmapImageRep alloc]
                                initWithBitmapDataPlanes:NULL
                                pixelsWide:p->w
                                pixelsHigh:p->h
                                bitsPerSample:8
                                samplesPerPixel:4
                                hasAlpha:YES
                                isPlanar:NO
                                colorSpaceName:NSCalibratedRGBColorSpace
                                bitmapFormat: 0//(NSBitmapFormat)img->codec_priv
                                bytesPerRow:p->w*4
                                bitsPerPixel:32
                                ];
    if(bitmap == nil) return -1;
    memcpy([bitmap bitmapData], data, p->w*p->h*4);

    NSBitmapImageFileType type = NSPNGFileType;


    if(strlen(name) > 4)
    {
        char *ext = (char*)&name[strlen(name) - 4];
        if(     !strncasecmp(ext, ".png",  3)) type = NSPNGFileType;
        else if(!strncasecmp(ext, "jpeg",  4)) type = NSJPEGFileType;
        else if(!strncasecmp(ext, ".jpg",  3)) type = NSJPEGFileType;
        else if(!strncasecmp(ext, ".bmp",  3)) type = NSBMPFileType;
        else if(!strncasecmp(ext, ".tif",  3)) type = NSTIFFFileType;
        else if(!strncasecmp(ext, ".tiff", 3)) type = NSTIFFFileType;
        else if(!strncasecmp(ext, ".gif",  3)) type = NSGIFFileType;
        else if(!strncasecmp(ext, ".bmp",  3)) type = NSBMPFileType;
        else if(!strncasecmp(ext, ".jp2",  3)) type = NSJPEG2000FileType;
        else if(!strncasecmp(ext, ".j2k",  3)) type = NSJPEG2000FileType;
    }

    [[bitmap representationUsingType:type properties:nil] writeToFile:n  atomically:YES];
    [autoreleasepool release];

    return 1;
}

/*
 * XXX: The following functions are local.
 */

static int pipi_free_coreimage(pipi_image_t *img)
{
    if(img->codec_priv)
        free(img->codec_priv);
    return 0;
}

#endif
