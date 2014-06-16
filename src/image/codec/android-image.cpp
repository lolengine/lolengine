//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined __ANDROID__

#include <jni.h>
#include <android/log.h>

extern "C" {
#include <android_native_app_glue.h>
}

#include "core.h"
#include "../../image/image-private.h"

using namespace std;

namespace lol
{

extern ANativeActivity *g_activity;

/*
 * Image implementation class
 */

DECLARE_IMAGE_CODEC(AndroidImageCodec, 100)
{
public:
    virtual bool Load(Image *image, char const *path);
    virtual bool Save(Image *image, char const *path);
    virtual bool Close();

    virtual uint8_t *GetData() const;

private:
    jobject bmp;
    jintArray array;
    jint *pixels;
};

bool AndroidImageCodec::Load(Image *image, char const *path)
{
    JNIEnv *env;
    jint res = g_activity->vm->GetEnv((void **)&env, JNI_VERSION_1_2);
    if (res < 0)
    {
#if !LOL_BUILD_RELEASE
        Log::Error("JVM environment not found, trying to attach thread\n");
#endif
        res = g_activity->vm->AttachCurrentThread(&env, nullptr);
    }
    if (res < 0)
    {
#if !LOL_BUILD_RELEASE
        Log::Error("JVM environment not found, cannot open image %s\n", path);
#endif
        return false;
    }
    jclass cls = env->GetObjectClass(g_activity->clazz);
    jmethodID mid;

    mid = env->GetMethodID(cls, "openImage",
                           "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
    jstring name = env->NewStringUTF(path);
    bmp = env->CallObjectMethod(g_activity->clazz, mid, name);
    env->DeleteLocalRef(name);
    if (!bmp)
    {
#if !LOL_BUILD_RELEASE
        Log::Error("could not load %s\n", path);
#endif
        return false;
    }
    env->NewGlobalRef(bmp);

    /* Get image dimensions */
    mid = env->GetMethodID(cls, "getWidth", "(Landroid/graphics/Bitmap;)I");
    m_size.x = env->CallIntMethod(g_activity->clazz, mid, bmp);
    mid = env->GetMethodID(cls, "getHeight", "(Landroid/graphics/Bitmap;)I");
    m_size.y = env->CallIntMethod(g_activity->clazz, mid, bmp);

    /* Get pixels */
    array = env->NewIntArray(m_size.x * m_size.y);
    env->NewGlobalRef(array);
    mid = env->GetMethodID(cls, "getPixels", "(Landroid/graphics/Bitmap;[I)V");
    env->CallVoidMethod(g_activity->clazz, mid, bmp, array);

    pixels = env->GetIntArrayElements(array, 0);
    for (int n = 0; n < m_size.x * m_size.y; n++)
    {
        uint32_t u = pixels[n];
        u = (u & 0xff00ff00) | ((u & 0xff0000) >> 16) | ((u & 0xff) << 16);
        pixels[n] = u;
    }
    m_format = PixelFormat::RGBA_8;

    return true;
}

bool AndroidImageCodec::Save(Image *image, char const *path)
{
    UNUSED(path);

    /* TODO: unimplemented */
}

bool AndroidImageCodec::Close()
{
    JNIEnv *env;
    jint res = g_activity->vm->GetEnv((void **)&env, JNI_VERSION_1_2);
    if (res < 0)
    {
#if !LOL_BUILD_RELEASE
        Log::Error("JVM environment not found, cannot close image\n");
#endif
        return false;
    }
    jclass cls = env->GetObjectClass(g_activity->clazz);
    jmethodID mid;

    env->ReleaseIntArrayElements(array, pixels, 0);
    env->DeleteGlobalRef(array);

    /* Free image */
    mid = env->GetMethodID(cls, "closeImage", "(Landroid/graphics/Bitmap;)V");
    env->CallVoidMethod(g_activity->clazz, mid, bmp);
    env->DeleteGlobalRef(bmp);

    return true;
}

uint8_t *AndroidImageCodec::GetData() const
{
    return (uint8_t *)pixels;
}

} /* namespace lol */

#endif /* __ANDROID__ */

