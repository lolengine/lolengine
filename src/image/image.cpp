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

#include <cmath>

#if defined __APPLE__ && defined __MACH__
#   import <UIKit/UIKit.h>
#elif defined USE_SDL
#   include <SDL.h>
#   include <SDL_image.h>
#elif defined __ANDROID__
#   include <jni.h>
#   include <android/log.h>
#elif defined __CELLOS_LV2__
#   include <cell/sysmodule.h>
#   include <cell/codec/pngdec.h>
#endif

#include "core.h"

using namespace std;

namespace lol
{

#if defined __ANDROID__
extern JavaVM *g_vm;
extern jobject g_activity;
#endif

/*
 * Image implementation class
 */

class ImageData
{
    friend class Image;

private:
    ivec2 size;
    Image::format_t format;

#if defined __APPLE__ && defined __MACH__
    uint8_t *pixels;
#elif defined USE_SDL
    SDL_Surface *img;
#elif defined __ANDROID__
    jobject bmp;
    jintArray array;
    jint *pixels;
#elif defined __CELLOS_LV2__
    static void* Malloc(uint32_t size, void* data) { return malloc(size); };
    static int32_t Free(void* ptr, void* data) { free(ptr); return 0; };
    uint8_t *pixels;
#else
    uint8_t *pixels;
#endif
};

/*
 * Public Image class
 */

Image::Image(char const *path)
  : data(new ImageData())
{
#if defined __APPLE__ && defined __MACH__
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
    data->size = ivec2(w, h);
    data->format = FORMAT_RGBA;

    CGColorSpaceRef cspace = CGColorSpaceCreateDeviceRGB();
    data->pixels = (uint8_t *)malloc(w * h * 4);
    CGContextRef ctx =
            CGBitmapContextCreate(data->pixels, w, h, 8, 4 * w, cspace,
                    kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(cspace);
    CGContextClearRect(ctx, CGRectMake(0, 0, w, h));
    CGContextTranslateCTM(ctx, 0, h - h);
    CGContextDrawImage(ctx, CGRectMake(0, 0, w, h), image.CGImage);
    CGContextRelease(ctx);
    [image release];
    [pngdata release];
#elif defined USE_SDL
    for (char const *name = path; *name; name++)
        if ((data->img = IMG_Load(name)))
            break;

    if (!data->img)
    {
#if !LOL_RELEASE
        Log::Error("could not load %s\n", path);
#endif
        SDL_Quit();
        exit(1);
    }

    data->size = ivec2(data->img->w, data->img->h);
    data->format = data->img->format->Amask ? FORMAT_RGBA : FORMAT_RGB;
#elif defined __ANDROID__
    JNIEnv *env;
    jint res = g_vm->GetEnv((void **)&env, JNI_VERSION_1_2);
    if (res < 0)
    {
#if !LOL_RELEASE
        Log::Error("could not get JVM environment\n");
#endif
        exit(1);
    }
    jclass cls = env->GetObjectClass(g_activity);
    jmethodID mid;

    mid = env->GetMethodID(cls, "openImage",
                           "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
    jstring name = env->NewStringUTF(path);
    data->bmp = env->CallObjectMethod(g_activity, mid, name);
    env->DeleteLocalRef(name);
    if (!data->bmp)
    {
#if !LOL_RELEASE
        Log::Error("could not load %s\n", path);
#endif
        exit(1);
    }
    env->NewGlobalRef(data->bmp);

    /* Get image dimensions */
    mid = env->GetMethodID(cls, "getWidth", "(Landroid/graphics/Bitmap;)I");
    data->size.x = env->CallIntMethod(g_activity, mid, data->bmp);
    mid = env->GetMethodID(cls, "getHeight", "(Landroid/graphics/Bitmap;)I");
    data->size.y = env->CallIntMethod(g_activity, mid, data->bmp);

    /* Get pixels */
    data->array = env->NewIntArray(data->size.x * data->size.y);
    env->NewGlobalRef(data->array);
    mid = env->GetMethodID(cls, "getPixels", "(Landroid/graphics/Bitmap;[I)V");
    env->CallVoidMethod(g_activity, mid, data->bmp, data->array);

    data->pixels = env->GetIntArrayElements(data->array, 0);
    for (int n = 0; n < data->size.x * data->size.y; n++)
    {
        uint32_t u = data->pixels[n];
        u = (u & 0xff00ff00) | ((u & 0xff0000) >> 16) | ((u & 0xff) << 16);
        data->pixels[n] = u;
    }
    data->format = FORMAT_RGBA;
#elif defined __CELLOS_LV2__
    int32_t err;

    /* Initialise decoding library */
    CellPngDecMainHandle hmain;

    err = cellSysmoduleLoadModule(CELL_SYSMODULE_FS);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not open Fs sysmodule\n");
#endif
        exit(1);
    }

    err = cellSysmoduleLoadModule(CELL_SYSMODULE_PNGDEC);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not open PngDec sysmodule\n");
#endif
        exit(1);
    }

    CellPngDecThreadInParam in_param;
    in_param.spuThreadEnable = CELL_PNGDEC_SPU_THREAD_ENABLE;
    in_param.ppuThreadPriority = 1000;
    in_param.spuThreadPriority = 200;
    in_param.cbCtrlMallocFunc = ImageData::Malloc;
    in_param.cbCtrlMallocArg = NULL;
    in_param.cbCtrlFreeFunc = ImageData::Free;
    in_param.cbCtrlFreeArg = NULL;
    CellPngDecThreadOutParam out_param;
    err = cellPngDecCreate(&hmain, &in_param, &out_param);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not create PngDec library\n");
#endif
        exit(1);
    }

    /* Create decoder */
    CellPngDecSubHandle hsub;

    char file[1024];
    sprintf(file, "/app_home/c:/Users/s.hocevar/lolengine/%s", path);

    CellPngDecSrc dec_src;
    dec_src.srcSelect = CELL_PNGDEC_FILE;
    dec_src.fileName = file;
    dec_src.fileOffset = 0;
    dec_src.fileSize = 0;
    dec_src.streamPtr = NULL;
    dec_src.streamSize = 0;
    dec_src.spuThreadEnable  = CELL_PNGDEC_SPU_THREAD_ENABLE;
    CellPngDecOpnInfo open_info;
    err = cellPngDecOpen(hmain, &hsub, &dec_src, &open_info);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not open %s for decoding\n", file);
#endif
        exit(1);
    }

    CellPngDecInfo info;
    err = cellPngDecReadHeader(hmain, hsub, &info);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not read image header\n");
#endif
        exit(1);
    }

    CellPngDecInParam in_dec_param;
    in_dec_param.commandPtr = NULL;
    in_dec_param.outputMode = CELL_PNGDEC_TOP_TO_BOTTOM;
    in_dec_param.outputColorSpace = CELL_PNGDEC_RGBA;
    in_dec_param.outputBitDepth = 8;
    in_dec_param.outputPackFlag = CELL_PNGDEC_1BYTE_PER_1PIXEL;
    in_dec_param.outputAlphaSelect = CELL_PNGDEC_STREAM_ALPHA;
    in_dec_param.outputColorAlpha = 0xff;
    CellPngDecOutParam out_dec_param;
    err = cellPngDecSetParameter(hmain, hsub, &in_dec_param, &out_dec_param);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not configure PngDec decoder\n");
#endif
        exit(1);
    }

    /* Decode image */
    data->size = ivec2(info.imageWidth, info.imageHeight);
    data->format = FORMAT_RGBA;
    data->pixels = (uint8_t *)malloc(info.imageWidth * 4 * info.imageHeight);
    CellPngDecDataCtrlParam data_ctrl_param;
    data_ctrl_param.outputBytesPerLine = info.imageWidth * 4;
    CellPngDecDataOutInfo data_out_info;
    err = cellPngDecDecodeData(hmain, hsub, data->pixels,
                               &data_ctrl_param, &data_out_info);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not run PngDec decoder\n");
#endif
        exit(1);
    }

    /* Close decoder */
    err = cellPngDecClose(hmain, hsub);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not close PngDec decoder\n");
#endif
        exit(1);
    }

    /* Deinitialise library */
    err = cellPngDecDestroy(hmain);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not destroy PngDec decoder\n");
#endif
        exit(1);
    }
    err = cellSysmoduleUnloadModule(CELL_SYSMODULE_PNGDEC);
    err = cellSysmoduleUnloadModule(CELL_SYSMODULE_FS);
#else
    data->size = 256;
    data->format = FORMAT_RGBA;
    data->pixels = (uint8_t *)malloc(256 * 256 * 4 * sizeof(*data->pixels));
    uint8_t *parser = data->pixels;
    for (int j = 0; j < 256; j++)
        for (int i = 0; i < 256; i++)
        {
            *parser++ = ((i ^ j) & 1) * 0xff;
            *parser++ = (uint8_t)i;
            *parser++ = (uint8_t)j;
            *parser++ = (((i >> 4) ^ (j >> 4)) & 1) * 0xff;
        }
#endif
}

ivec2 Image::GetSize() const
{
    return data->size;
}

Image::format_t Image::GetFormat() const
{
    return data->format;
}

void * Image::GetData() const
{
#if defined __APPLE__ && defined __MACH__
    return data->pixels;
#elif defined USE_SDL
    return data->img->pixels;
#elif defined __ANDROID__
    return data->pixels;
#elif defined __CELLOS_LV2__
    return data->pixels;
#else
    return data->pixels;
#endif
}

Image::~Image()
{
#if defined __APPLE__ && defined __MACH__
    free(data->pixels);
#elif defined USE_SDL
    SDL_FreeSurface(data->img);
#elif defined __ANDROID__
    JNIEnv *env;
    jint res = g_vm->GetEnv((void **)&env, JNI_VERSION_1_2);
    if (res < 0)
    {
#if !LOL_RELEASE
        Log::Error("could not get JVM environment\n");
#endif
        exit(1);
    }
    jclass cls = env->GetObjectClass(g_activity);
    jmethodID mid;

    env->ReleaseIntArrayElements(data->array, data->pixels, 0);
    env->DeleteGlobalRef(data->array);

    /* Free image */
    mid = env->GetMethodID(cls, "closeImage", "(Landroid/graphics/Bitmap;)V");
    env->CallVoidMethod(g_activity, mid, data->bmp);
    env->DeleteGlobalRef(data->bmp);
#elif defined __CELLOS_LV2__
    free(data->pixels);
#else
    free(data->pixels);
#endif
    delete data;
}

} /* namespace lol */

