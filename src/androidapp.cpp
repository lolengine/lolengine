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

    new DebugQuad();
    new DebugQuad();
    //new Interface();
    new DebugFps(20, 20);
}

extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeResize(JNIEnv* env, jobject that,
                                                jint w, jint h)
{
    __android_log_print(ANDROID_LOG_INFO, "LOL", "resize w=%d h=%d", w, h);
    Video::Setup(w, h);
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

