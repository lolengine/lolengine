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

#include <jni.h>
#include <android/log.h>

#include "core.h"
#include "lolgl.h"
#include "loldebug.h"

using namespace lol;

#include "interface.h"

extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeInit(JNIEnv* env)
{
    __android_log_print(ANDROID_LOG_INFO, "LOL", "init");
    Ticker::Setup(10.0f);
    Video::Setup(320, 200);

    new Interface();
    new DebugFps(20, 20);
}

static void TestPng(JNIEnv *env)
{
    jclass cls = env->FindClass("org/zoy/LolEngine/LolActivity");
    jmethodID mid;

    /* Open image from resources */
    mid = env->GetStaticMethodID(cls, "openImage", "(Ljava/lang/String;)V");
    jstring name = env->NewStringUTF("ascii");
    env->CallStaticVoidMethod(cls, mid, name);
    env->DeleteLocalRef(name);

    /* Get image dimensions */
    mid = env->GetStaticMethodID(cls, "getWidth", "()I");
    jint w = env->CallStaticIntMethod(cls, mid);
    mid = env->GetStaticMethodID(cls, "getHeight", "()I");
    jint h = env->CallStaticIntMethod(cls, mid);
    __android_log_print(ANDROID_LOG_INFO, "LOL", "Loaded PNG %dx%d", w, h);

    /* Get pixels */
    jintArray arr = env->NewIntArray(w * h * 4);
    mid = env->GetStaticMethodID(cls, "getPixels", "([I)V");
    env->CallStaticVoidMethod(cls, mid, arr);

    /* Do something with the pixels */
    jint *data = env->GetIntArrayElements(arr, 0);
    __android_log_print(ANDROID_LOG_INFO, "LOL", "pixels %08x %08x %08x %08x",
                        (unsigned)data[0], (unsigned)data[1],
                        (unsigned)data[2], (unsigned)data[3]);
    env->ReleaseIntArrayElements(arr, data, 0);
    env->DeleteLocalRef(arr);

    /* Free image */
    mid = env->GetStaticMethodID(cls, "closeImage", "()V");
    env->CallStaticVoidMethod(cls, mid);
}

extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeResize(JNIEnv* env, jobject thiz,
                                                jint w, jint h)
{
    __android_log_print(ANDROID_LOG_INFO, "LOL", "resize w=%d h=%d", w, h);
    Video::Setup(w, h);

    TestPng(env);
}

extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeDone(JNIEnv* env)
{
    /* FIXME: clean up */
}

extern "C" void
Java_org_zoy_LolEngine_LolGLSurfaceView_nativePause(JNIEnv* env)
{
    /* TODO: unimplemented */
}

/* Call to render the next GL frame */
extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeRender(JNIEnv* env)
{
    Ticker::ClampFps();
    Ticker::TickGame();
    Ticker::TickDraw();
}

