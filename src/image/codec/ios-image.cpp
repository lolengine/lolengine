//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined __APPLE__ && defined __MACH__

#include <cmath>
#import <UIKit/UIKit.h>

#include "core.h"

using namespace std;

namespace lol
{

/*
 * Image implementation class
 */

DECLARE_IMAGE_LOADER(IosImageData, 100)
{
public:
    virtual bool Open(char const *);
    virtual bool Close();

    virtual void *GetData() const;

private:
    uint8_t *pixels;
};

/*
 * Public Image class
 */

bool IosImageData::Open(char const *path)
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
#if !LOL_RELEASE
        Log::Error("could not load %s\n", path);
#endif
        exit(1);
    }

    int w = CGImageGetWidth(image.CGImage);
    int h = CGImageGetHeight(image.CGImage);
    size = ivec2(w, h);
    format = FORMAT_RGBA;

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

bool IosImageData::Close()
{
    free(pixels);

    return true;
}

void * IosImageData::GetData() const
{
    return pixels;
}

} /* namespace lol */

#endif /* defined __APPLE__ && defined __MACH__ */

