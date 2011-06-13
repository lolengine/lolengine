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

namespace lol
{
jobject g_ctx;
JNIEnv *g_env;
};

extern "C" void
Java_org_zoy_LolEngine_LolActivity_nativeInit(JNIEnv* env, jobject thiz)
{
    env->NewGlobalRef(thiz); /* FIXME: never released! */
    g_ctx = thiz;
}

extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeInit(JNIEnv* env)
{
    g_env = env;

    __android_log_print(ANDROID_LOG_INFO, "LOL", "init");
    Ticker::Setup(30.0f);
    Video::Setup(vec2i(320, 200));

    new Interface();
    new DebugFps(20, 20);
}

extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeResize(JNIEnv* env, jobject thiz,
                                                jint w, jint h)
{
    __android_log_print(ANDROID_LOG_INFO, "LOL", "resize w=%d h=%d", w, h);
    Video::Setup(vec2i(w, h));
}

extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeDone(JNIEnv* env)
{
    /* FIXME: clean up */
}

extern "C" void
Java_org_zoy_LolEngine_LolView_nativePause(JNIEnv* env)
{
    /* TODO: unimplemented */
}

extern "C" void
Java_org_zoy_LolEngine_LolView_nativeDown(JNIEnv* env)
{
    Input::SetMouseButton(0);
}

extern "C" void
Java_org_zoy_LolEngine_LolView_nativeUp(JNIEnv* env)
{
    Input::UnsetMouseButton(0);
}

extern "C" void
Java_org_zoy_LolEngine_LolView_nativeMove(JNIEnv* env, jobject thiz,
                                          jint x, jint y)
{
    vec2i pos = vec2i(0, 479) + vec2i(x * 640, -y * 480) / Video::GetSize();
    Input::SetMousePos(pos);
}

/* Call to render the next GL frame */
extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeRender(JNIEnv* env)
{
    Ticker::ClampFps();
    Ticker::TickGame();
    Ticker::TickDraw();
}

