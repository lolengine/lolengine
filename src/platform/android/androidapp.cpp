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

#include <jni.h>
#include <android/log.h>

#include "core.h"
#include "loldebug.h"
#include "androidapp.h"

using namespace lol;

/* Monsterz-specific */
#include "interface.h"

namespace lol
{
JavaVM *g_vm;
jobject g_activity;

AndroidApp::AndroidApp(char const *title, ivec2 res, float fps) :
    data(0)
{
}

void AndroidApp::ShowPointer(bool show)
{
}

void AndroidApp::Run()
{
    while (!Ticker::Finished())
    {
        /* Tick the renderer, show the frame and clamp to desired framerate. */
        Ticker::TickDraw();
    }
}

AndroidApp::~AndroidApp()
{
}

};

extern "C" jint
JNI_OnLoad(JavaVM* vm, void* reserved)
{
    g_vm = vm;
    return JNI_VERSION_1_4;
}

extern "C" void
Java_org_zoy_LolEngine_LolActivity_nativeInit(JNIEnv* env, jobject thiz)
{
    env->NewGlobalRef(thiz); /* FIXME: never released! */
    g_activity = thiz;
}

extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeInit(JNIEnv* env)
{
    Log::Info("initialising renderer");
    Ticker::Setup(30.0f);
    Video::Setup(ivec2(320, 200));

    new Interface();
    new DebugFps(20, 20);
}

extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeResize(JNIEnv* env, jobject thiz,
                                                jint w, jint h)
{
    Log::Info("resizing to %i x %i", w, h);
    Video::Setup(ivec2(w, h));
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
    ivec2 pos = ivec2(0, 479) + ivec2(x * 640, -y * 480) / Video::GetSize();
    Input::SetMousePos(pos);
}

/* Call to render the next GL frame */
extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeRender(JNIEnv* env)
{
    Ticker::TickDraw();
}

#endif /* __ANDROID__ */

