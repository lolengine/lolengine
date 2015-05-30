//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if defined __APPLE__ && defined __MACH__ && defined __arm__

#import <UIKit/UIKit.h>

#include "../../image/image-private.h"

namespace lol
{

/*
 * Image implementation class
 */

class IosImageCodec : public ImageCodec
{
public:
    virtual char const *GetName() { return "<IosImageCodec>"; }
    virtual bool Load(Image *image, char const *path);
    virtual bool Save(Image *image, char const *path);
};

DECLARE_IMAGE_CODEC(IosImageCodec, 100)

/*
 * Public Image class
 */

bool IosImageCodec::Load(Image *image, char const *path)
{
    NSString *fullpath = [NSString stringWithUTF8String:path];
    NSArray *chunks = [fullpath componentsSeparatedByString: @"/"];
    NSString *filename = [chunks objectAtIndex: [chunks count] - 1];
    chunks = [filename componentsSeparatedByString: @"."];
    NSString *prefix = [chunks objectAtIndex: 0];
    NSString *mypath = [[NSBundle mainBundle] pathForResource:prefix ofType:@"png"];
    NSData *pngdata = [[NSData alloc] initWithContentsOfFile:mypath];
    UIImage *image = [[UIImage alloc] initWithData:pngdata];
    if (!image)
    {
#if !LOL_BUILD_RELEASE
        msg::error("could not load %s\n", path);
#endif
        return false;
    }

    int w = CGImageGetWidth(image.CGImage);
    int h = CGImageGetHeight(image.CGImage);
    m_size = ivec2(w, h);
    m_format = PixelFormat::RGBA_8;

    CGColorSpaceRef cspace = CGColorSpaceCreateDeviceRGB();
    pixels = (uint8_t *)malloc(w * h * 4);
    CGContextRef ctx =
            CGBitmapContextCreate(pixels, w, h, 8, 4 * w, cspace,
                    kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(cspace);
    CGContextClearRect(ctx, CGRectMake(0, 0, w, h));
    CGContextTranslateCTM(ctx, 0, h - h);
    CGContextDrawImage(ctx, CGRectMake(0, 0, w, h), image.CGImage);
    CGContextRelease(ctx);
    [image release];
    [pngdata release];

    return true;
}

bool IosImageCodec::Save(Image *image, char const *path)
{
    UNUSED(path);

    /* TODO: unimplemented */
    return true;
}

bool IosImageCodec::Close()
{
    free(pixels);

    return true;
}

uint8_t * IosImageCodec::GetData() const
{
    return pixels;
}

} /* namespace lol */

#endif /* defined __APPLE__ && defined __MACH__ && defined __arm__ */

