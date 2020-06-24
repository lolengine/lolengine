//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if defined __ANDROID__

#include <string>

#include <jni.h>
#include <android/log.h>

extern "C" {
#include <android_native_app_glue.h>
}

#include "../../image/resource-private.h"

namespace lol
{

extern ANativeActivity *g_activity;

/*
 * Image implementation class
 */

class AndroidImageCodec : public ResourceCodec
{
public:
    virtual std::string GetName() { return "<AndroidImageCodec>"; }
    virtual ResourceCodecData* Load(std::string const &path);
    virtual bool Save(std::string const &path, ResourceCodecData* data);
    virtual bool Close();

    virtual uint8_t *GetData() const;

private:
    jobject m_bmp;
    jintArray m_array;
    jint *m_pixels;
};

DECLARE_IMAGE_CODEC(AndroidImageCodec, 100)

ResourceCodecData* AndroidImageCodec::Load(std::string const &path)
{
    JNIEnv *env;
    jint res = g_activity->vm->GetEnv((void **)&env, JNI_VERSION_1_2);
    if (res < 0)
    {
#if !LOL_BUILD_RELEASE
        msg::error("JVM environment not found, trying to attach thread\n");
#endif
        res = g_activity->vm->AttachCurrentThread(&env, nullptr);
    }
    if (res < 0)
    {
#if !LOL_BUILD_RELEASE
        msg::error("JVM environment not found, cannot open image %s\n", path.c_str());
#endif
        return nullptr;
    }
    jclass cls = env->GetObjectClass(g_activity->clazz);
    jmethodID mid;

    mid = env->GetMethodID(cls, "openImage",
                           "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
    jstring name = env->NewStringUTF(path.c_str());
    m_bmp = env->CallObjectMethod(g_activity->clazz, mid, name);
    env->DeleteLocalRef(name);
    if (!m_bmp)
    {
#if !LOL_BUILD_RELEASE
        msg::error("could not load %s\n", path.c_str());
#endif
        return nullptr;
    }
    env->NewGlobalRef(m_bmp);

    // Get image dimensions
    mid = env->GetMethodID(cls, "getWidth", "(Landroid/graphics/Bitmap;)I");
    int width = env->CallIntMethod(g_activity->clazz, mid, m_bmp);
    mid = env->GetMethodID(cls, "getHeight", "(Landroid/graphics/Bitmap;)I");
    int height = env->CallIntMethod(g_activity->clazz, mid, m_bmp);
    ivec2 size(width, height);

    auto data = new ResourceImageData(new old_image(size));

    // Get pixels
    m_array = env->NewIntArray(size.x * size.y);
    env->NewGlobalRef(m_array);
    mid = env->GetMethodID(cls, "getPixels", "(Landroid/graphics/Bitmap;[I)V");
    env->CallVoidMethod(g_activity->clazz, mid, m_bmp, m_array);

    m_pixels = env->GetIntArrayElements(m_array, 0);
    for (int n = 0; n < size.x * size.y; n++)
    {
        uint32_t u = m_pixels[n];
        u = (u & 0xff00ff00) | ((u & 0xff0000) >> 16) | ((u & 0xff) << 16);
        m_pixels[n] = u;
    }

    return data;
}

bool AndroidImageCodec::Save(std::string const &path, ResourceCodecData* data)
{
    /* TODO: unimplemented */
    (void)path;
    (void)data;
    return false;
}

bool AndroidImageCodec::Close()
{
    JNIEnv *env;
    jint res = g_activity->vm->GetEnv((void **)&env, JNI_VERSION_1_2);
    if (res < 0)
    {
#if !LOL_BUILD_RELEASE
        msg::error("JVM environment not found, cannot close image\n");
#endif
        return false;
    }
    jclass cls = env->GetObjectClass(g_activity->clazz);
    jmethodID mid;

    env->ReleaseIntArrayElements(m_array, m_pixels, 0);
    env->DeleteGlobalRef(m_array);

    /* Free image */
    mid = env->GetMethodID(cls, "closeImage", "(Landroid/graphics/Bitmap;)V");
    env->CallVoidMethod(g_activity->clazz, mid, m_bmp);
    env->DeleteGlobalRef(m_bmp);

    return true;
}

uint8_t *AndroidImageCodec::GetData() const
{
    return (uint8_t *)m_pixels;
}

} /* namespace lol */

#endif /* __ANDROID__ */

