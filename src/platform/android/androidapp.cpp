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

#include "core.h"
#include "loldebug.h"
#include "androidapp.h"

using namespace lol;

/* One of these wrappers will be overridden by the user's version */
void lol_android_main(void) __attribute__((weak));
void lol_android_main(int argc, char **argv) __attribute__((weak));
void lol_android_main(int argc, char **argv, char **envp) __attribute__((weak));

namespace lol
{
JavaVM *g_vm;
jobject g_activity;
Queue<int> g_main_queue;
Thread *g_main_thread;
float g_fps;

AndroidApp::AndroidApp(char const *title, ivec2 res, float fps)
  : m_data(0)
{
    g_fps = fps;
}

void AndroidApp::ShowPointer(bool show)
{
}

/* This is a fake Run() method. We just wait until we're called and
 * signal nativeInit() that all the user's initialisation code was
 * called. Then we sit here forever, the Java layer is in charge of
 * calling TickDraw(). */
void AndroidApp::Run()
{
    g_main_queue.Push(1);
    g_main_queue.Push(1);

    while (!Ticker::Finished())
    {
        /* Do nothing while the real render thread does the job. The
         * real stuff happens in nativeRender() */
        Timer t;
        t.Wait(0.5f);
    }
}

void *AndroidApp::MainRun(void *data)
{
    int argc = 1;
    char *argv[] = { "", NULL };
    char *env[] = { NULL };

    /* Call the user's main() function. One of these will work. */
    lol_android_main();
    lol_android_main(argc, argv);
    lol_android_main(argc, argv, env);

    return NULL;
}

AndroidApp::~AndroidApp()
{
}

};

extern "C" jint
JNI_OnLoad(JavaVM* vm, void* reserved)
{
    Log::Info("Java layer loading library, vm=0x%08lx", (long)(intptr_t)vm);
    g_vm = vm;
    return JNI_VERSION_1_4;
}

extern "C" void
Java_org_zoy_LolEngine_LolActivity_nativeInit(JNIEnv* env, jobject thiz)
{
    Log::Info("Java layer initialising activity 0x%08lx", (long)thiz);
    env->NewGlobalRef(thiz); /* FIXME: never released! */
    g_activity = thiz;
}

extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeInit(JNIEnv* env)
{
    /* Initialise app thread and wait for it to be ready, ie. set
     * the FPS value at least. */
    g_main_thread = new Thread(lol::AndroidApp::MainRun, NULL);;
    g_main_queue.Pop();

    /* Launch our ticker */
    Log::Info("Java layer initialising renderer at %g fps", g_fps);
    Ticker::Setup(g_fps);
    Video::Setup(ivec2(320, 200));

    /* Wake up app thread */
    g_main_queue.Pop();
}

extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeResize(JNIEnv* env, jobject thiz,
                                                jint w, jint h)
{
    Log::Info("Java layer resizing to %i x %i", w, h);
    Video::Setup(ivec2(w, h));
}

extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeDone(JNIEnv* env)
{
    /* FIXME: clean up */
    delete g_main_thread;
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
    Input::SetMousePos(ivec2(x, y));
}

/* Call to render the next GL frame */
extern "C" void
Java_org_zoy_LolEngine_LolRenderer_nativeRender(JNIEnv* env)
{
    Ticker::TickDraw();
}

/*
 * Fake main() wrappers that let us call the user’s main() from within
 * a separate thread.
 */
void lol_android_main(void) {}
void lol_android_main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
}
void lol_android_main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    (void)envp;
}

#endif /* __ANDROID__ */

