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

#if defined __ANDROID__

#include <cmath>

#include <jni.h>
#include <android/log.h>

#include "core.h"
#include "../../image/image-private.h"

using namespace std;

namespace lol
{

extern JavaVM *g_vm;
extern jobject g_activity;

/*
 * Image implementation class
 */

DECLARE_IMAGE_LOADER(AndroidImageData, 100)
{
public:
    virtual bool Open(char const *);
    virtual bool Close();

    virtual void *GetData() const;

private:
    jobject bmp;
    jintArray array;
    jint *pixels;
};

bool AndroidImageData::Open(char const *path)
{
    JNIEnv *env;
    jint res = g_vm->GetEnv((void **)&env, JNI_VERSION_1_2);
    if (res < 0)
    {
#if !LOL_RELEASE
        Log::Error("could not get JVM environment\n");
#endif
        return false;
    }
    jclass cls = env->GetObjectClass(g_activity);
    jmethodID mid;

    mid = env->GetMethodID(cls, "openImage",
                           "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
    jstring name = env->NewStringUTF(path);
    bmp = env->CallObjectMethod(g_activity, mid, name);
    env->DeleteLocalRef(name);
    if (!bmp)
    {
#if !LOL_RELEASE
        Log::Error("could not load %s\n", path);
#endif
        return false;
    }
    env->NewGlobalRef(bmp);

    /* Get image dimensions */
    mid = env->GetMethodID(cls, "getWidth", "(Landroid/graphics/Bitmap;)I");
    size.x = env->CallIntMethod(g_activity, mid, bmp);
    mid = env->GetMethodID(cls, "getHeight", "(Landroid/graphics/Bitmap;)I");
    size.y = env->CallIntMethod(g_activity, mid, bmp);

    /* Get pixels */
    array = env->NewIntArray(size.x * size.y);
    env->NewGlobalRef(array);
    mid = env->GetMethodID(cls, "getPixels", "(Landroid/graphics/Bitmap;[I)V");
    env->CallVoidMethod(g_activity, mid, bmp, array);

    pixels = env->GetIntArrayElements(array, 0);
    for (int n = 0; n < size.x * size.y; n++)
    {
        uint32_t u = pixels[n];
        u = (u & 0xff00ff00) | ((u & 0xff0000) >> 16) | ((u & 0xff) << 16);
        pixels[n] = u;
    }
    format = Image::FORMAT_RGBA;

    return true;
}

bool AndroidImageData::Close()
{
    JNIEnv *env;
    jint res = g_vm->GetEnv((void **)&env, JNI_VERSION_1_2);
    if (res < 0)
    {
#if !LOL_RELEASE
        Log::Error("could not get JVM environment\n");
#endif
        return false;
    }
    jclass cls = env->GetObjectClass(g_activity);
    jmethodID mid;

    env->ReleaseIntArrayElements(array, pixels, 0);
    env->DeleteGlobalRef(array);

    /* Free image */
    mid = env->GetMethodID(cls, "closeImage", "(Landroid/graphics/Bitmap;)V");
    env->CallVoidMethod(g_activity, mid, bmp);
    env->DeleteGlobalRef(bmp);

    return true;
}

void * AndroidImageData::GetData() const
{
    return pixels;
}

} /* namespace lol */

#endif /* __ANDROID__ */

