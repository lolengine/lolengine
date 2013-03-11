//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
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

#include <EGL/egl.h>
#include <GLES/gl.h>

extern "C" {
#include <android_native_app_glue.h>
#include <android_native_app_glue.c>
}

#include "core.h"
#include "androidapp.h"

using namespace lol;

namespace lol
{
JavaVM *g_vm;
jobject g_activity;
}; /* namespace lol */

extern "C" jint
JNI_OnLoad(JavaVM* vm, void* reserved)
{
    Log::Info("Java layer loading library, vm=0x%08lx", (long)(intptr_t)vm);
    g_vm = vm;
    return JNI_VERSION_1_4;
}

extern "C" void
Java_net_lolengine_LolActivity_nativeInit(JNIEnv* env, jobject thiz)
{
    Log::Info("Java layer initialising activity 0x%08lx", (long)thiz);
    env->NewGlobalRef(thiz); /* FIXME: never released! */
    g_activity = thiz;
}

/* One of these wrappers will be overridden by the user's version */
void lol_android_main(void) __attribute__((weak));
void lol_android_main(int argc, char **argv) __attribute__((weak));
void lol_android_main(int argc, char **argv, char **envp) __attribute__((weak));

/**
 * Our saved state data.
 */
struct SavedState
{
    ivec2 position;
};

/**
 * Shared state for our app.
 */
class lol::AndroidAppData
{
public:
    android_app* m_native_app;

    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;

    SavedState m_state;
};

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct AndroidAppData* engine)
{
    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BUFFER_SIZE, 16,
        EGL_DEPTH_SIZE, 16,
        EGL_RED_SIZE, 4,
        EGL_GREEN_SIZE, 4,
        EGL_BLUE_SIZE, 4,
        EGL_ALPHA_SIZE, 4,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->m_native_app->window,
                                     0, 0, format);
    surface = eglCreateWindowSurface(display, config,
                                     engine->m_native_app->window, nullptr);

    EGLint ctxattr[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxattr);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
    {
        Log::Error("unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;

    /* Launch our ticker */
    Log::Info("Java layer initialising renderer at %g fps", 60.0f);
    Ticker::Setup(60.0f);
    Video::Setup(ivec2(w, h));

    return 0;
}

static void engine_draw_frame(AndroidAppData* engine)
{
    if (!engine->display)
        return;

    Ticker::TickDraw();

    eglSwapBuffers(engine->display, engine->surface);
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(AndroidAppData* engine)
{
    if (engine->display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(android_app* native_app, AInputEvent* event)
{
    AndroidAppData* engine = (AndroidAppData*)native_app->userData;
    switch (AInputEvent_getType(event))
    {
    case AINPUT_EVENT_TYPE_MOTION:
        Input::SetMousePos(ivec2(AMotionEvent_getX(event, 0),
                                 AMotionEvent_getY(event, 0)));
        switch (AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK)
        {
        case AMOTION_EVENT_ACTION_DOWN:
            Input::SetMouseButton(0);
            break;
        case AMOTION_EVENT_ACTION_UP:
            Input::UnsetMouseButton(0);
            break;
        }
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(android_app* native_app, int32_t cmd)
{
    AndroidAppData* engine = (AndroidAppData*)native_app->userData;
    switch (cmd)
    {
        case APP_CMD_SAVE_STATE:
            /* The system has asked us to save our current state. Do so. */
            engine->m_native_app->savedState = malloc(sizeof(SavedState));
            *((SavedState*)engine->m_native_app->savedState) = engine->m_state;
            engine->m_native_app->savedStateSize = sizeof(SavedState);
            break;
        case APP_CMD_INIT_WINDOW:
            /* The window is being shown, get it ready. */
            if (engine->m_native_app->window != nullptr)
            {
                engine_init_display(engine);
                engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            /* The window is being hidden or closed, clean it up. */
            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
            break;
        case APP_CMD_LOST_FOCUS:
            /* FIXME: stop animating */
            engine_draw_frame(engine);
            break;
    }
}

/* FIXME: find a better way to pass this to the AndroidApp ctor. */
AndroidAppData *g_data;

void android_main(android_app* native_app)
{
    g_data = new AndroidAppData();

    /* Make sure glue isn't stripped */
    app_dummy();

    native_app->userData = g_data;
    native_app->onAppCmd = engine_handle_cmd;
    native_app->onInputEvent = engine_handle_input;
    g_data->m_native_app = native_app;

    if (native_app->savedState != nullptr)
    {
        /* We are starting with a previous saved state; restore from it */
        g_data->m_state = *(SavedState*)native_app->savedState;
    }

    int argc = 1;
    char *argv[] = { "", nullptr };
    char *env[] = { nullptr };

    /* Call the user's main() function. One of these will work. */
    lol_android_main();
    lol_android_main(argc, argv);
    lol_android_main(argc, argv, env);
}

lol::AndroidApp::AndroidApp(char const *title, ivec2 res, float fps)
  : m_data(g_data)
{
    ;
}

void lol::AndroidApp::ShowPointer(bool show)
{
}

lol::AndroidApp::~AndroidApp()
{
    engine_term_display(m_data);
    delete m_data;
}

void lol::AndroidApp::Tick()
{
    /* Read all pending events. */
    int ident;
    int events;
    struct android_poll_source* source;

    /* Loop until all events are read, then continue to draw the next
     * frame of animation. */
    while ((ident = ALooper_pollAll(0, nullptr, &events,
                                    (void**)&source)) >= 0)
    {
        /* Process this event */
        if (source)
            source->process(m_data->m_native_app, source);

        /* Check if we are exiting */
        if (m_data->m_native_app->destroyRequested != 0)
            Ticker::Shutdown();
    }

    engine_draw_frame(m_data);
}

/*
 * Fake main() wrappers that let us call the user’s main() from within
 * a separate thread.
 */
void lol_android_main(void)
{
}

void lol_android_main(int argc, char **argv)
{
    UNUSED(argc, argv);
}

void lol_android_main(int argc, char **argv, char **envp)
{
    UNUSED(argc, argv, envp);
}

#endif /* __ANDROID__ */

