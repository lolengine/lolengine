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
#include <cstdio>

#if defined USE_SDL
#   include <SDL.h>
#   include <SDL_image.h>
#elif defined ANDROID_NDK
#   include <jni.h>
#   include <android/log.h>
#endif

#include "core.h"

namespace lol
{

#if defined ANDROID_NDK
extern JNIEnv *g_env;
extern jobject g_ctx;
#endif

/*
 * Image implementation class
 */

class ImageData
{
    friend class Image;

private:
    vec2i size;
    Image::format_t format;

#if defined USE_SDL
    SDL_Surface *img;
#elif defined ANDROID_NDK
    jobject bmp;
    jintArray array;
    jint *pixels;
#else
    uint8_t *dummy;
#endif
};

/*
 * Public Image class
 */

Image::Image(char const *path)
  : data(new ImageData())
{
#if defined USE_SDL
    for (char const *name = path; *name; name++)
        if ((data->img = IMG_Load(name)))
            break;

    if (!data->img)
    {
#if !LOL_RELEASE
        fprintf(stderr, "ERROR: could not load %s\n", path);
#endif
        SDL_Quit();
        exit(1);
    }

    data->size = vec2i(data->img->w, data->img->h);
    data->format = data->img->format->Amask ? FORMAT_RGBA : FORMAT_RGB;
#elif defined ANDROID_NDK
    jclass cls = g_env->GetObjectClass(g_ctx);
    jmethodID mid;

    mid = g_env->GetMethodID(cls, "openImage",
                             "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
    jstring name = g_env->NewStringUTF(path);
    data->bmp = g_env->CallObjectMethod(g_ctx, mid, name);
    g_env->DeleteLocalRef(name);
    g_env->NewGlobalRef(data->bmp);

    /* Get image dimensions */
    mid = g_env->GetMethodID(cls, "getWidth", "(Landroid/graphics/Bitmap;)I");
    data->size.x = g_env->CallIntMethod(g_ctx, mid, data->bmp);
    mid = g_env->GetMethodID(cls, "getHeight", "(Landroid/graphics/Bitmap;)I");
    data->size.y = g_env->CallIntMethod(g_ctx, mid, data->bmp);

    /* Get pixels */
    data->array = g_env->NewIntArray(data->size.x * data->size.y);
    g_env->NewGlobalRef(data->array);
    mid = g_env->GetMethodID(cls, "getPixels", "(Landroid/graphics/Bitmap;[I)V");
    g_env->CallVoidMethod(g_ctx, mid, data->bmp, data->array);

    data->pixels = g_env->GetIntArrayElements(data->array, 0);
    for (int n = 0; n < data->size.x * data->size.y; n++)
    {
        uint32_t u = data->pixels[n];
        u = (u & 0xff00ff00) | ((u & 0xff0000) >> 16) | ((u & 0xff) << 16);
        data->pixels[n] = u;
    }
    data->format = FORMAT_RGBA;
#else
    data->size = 256;
    data->format = FORMAT_RGBA;
    data->dummy = (uint8_t *)malloc(256 * 256 * 4 * sizeof(*data->dummy));
    uint8_t *parser = data->dummy;
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

vec2i Image::GetSize() const
{
    return data->size;
}

Image::format_t Image::GetFormat() const
{
    return data->format;
}

void * Image::GetData() const
{
#if defined USE_SDL
    return data->img->pixels;
#elif defined ANDROID_NDK
    return data->pixels;
#else
    return data->dummy;
#endif
}

Image::~Image()
{
#if defined USE_SDL
    SDL_FreeSurface(data->img);
#elif defined ANDROID_NDK
    jclass cls = g_env->GetObjectClass(g_ctx);
    jmethodID mid;

    g_env->ReleaseIntArrayElements(data->array, data->pixels, 0);
    g_env->DeleteGlobalRef(data->array);

    /* Free image */
    mid = g_env->GetMethodID(cls, "closeImage", "(Landroid/graphics/Bitmap;)V");
    g_env->CallVoidMethod(g_ctx, mid, data->bmp);
    g_env->DeleteGlobalRef(data->bmp);
#else
    free(data->dummy);
#endif
    delete data;
}

} /* namespace lol */

