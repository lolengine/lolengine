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
#include <android/asset_manager_jni.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

extern "C" {
#include <android_native_app_glue.h>
#include <android_native_app_glue.c>
}

#include "core.h"
#include "androidapp.h"

#include "input/input_internal.h"

using namespace lol;

namespace lol
{
JavaVM *g_vm;
jobject g_activity;
AAssetManager *g_assets;
}; /* namespace lol */

extern "C" jint
JNI_OnLoad(JavaVM* vm, void* reserved)
{
    Log::Info("Java layer loading library, vm=0x%08lx", (long)(intptr_t)vm);
    g_vm = vm;
    return JNI_VERSION_1_4;
}

extern "C" void
Java_net_lolengine_LolActivity_nativeInit(JNIEnv* env, jobject thiz,
                                          jobject assets)
{
    Log::Info("Java layer initialising activity 0x%08lx", (long)thiz);
    env->NewGlobalRef(thiz); /* FIXME: never released! */
    g_activity = thiz;
    env->NewGlobalRef(assets); /* FIXME: never released! */
    g_assets = AAssetManager_fromJava(env, assets);
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
    int CreateDisplay();
    void DestroyDisplay();
    void DrawFrame();

    static void StaticHandleCommand(android_app* native_app, int32_t cmd)
    {
        return ((AndroidAppData*)native_app->userData)->HandleCommand(cmd);
    }

    static int32_t StaticHandleInput(android_app* native_app, AInputEvent* ev)
    {
        return ((AndroidAppData*)native_app->userData)->HandleInput(ev);
    }

    android_app* m_native_app;

    /* The resolution that was asked (not the one we got) */
    /* FIXME: we need proper unproject or at least screen space events!! */
    ivec2 m_wanted_resolution;

    InputDeviceInternal* m_mouse;

    SavedState m_state;

private:
    void HandleCommand(int32_t cmd);
    int32_t HandleInput(AInputEvent* event);

    vec2 m_prev_pos;

    EGLDisplay m_display;
    EGLSurface m_surface;
    EGLContext m_context;
};

/**
 * Initialize an EGL context for the current display.
 */
int lol::AndroidAppData::CreateDisplay()
{
    /* FIXME: there is a lot of code common to eglapp.cpp here. */
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

    m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(m_display, 0, 0);
    eglChooseConfig(m_display, attribs, &config, 1, &numConfigs);
    eglGetConfigAttrib(m_display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(m_native_app->window,
                                     0, 0, format);
    m_surface = eglCreateWindowSurface(m_display, config,
                                       m_native_app->window, nullptr);

    EGLint ctxattr[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, ctxattr);

    if (eglMakeCurrent(m_display, m_surface, m_surface, m_context) == EGL_FALSE)
    {
        Log::Error("unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(m_display, m_surface, EGL_WIDTH, &w);
    eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &h);

    /* Launch our ticker */
    Log::Info("Java layer initialising renderer at %g fps", 60.0f);
    Ticker::Setup(60.0f);
    Video::Setup(ivec2(w, h));

    return 0;
}

void lol::AndroidAppData::DrawFrame()
{
    if (!m_display)
        return;

    Ticker::TickDraw();

    eglSwapBuffers(m_display, m_surface);
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void lol::AndroidAppData::DestroyDisplay()
{
    if (m_display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (m_context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(m_display, m_context);
        }
        if (m_surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(m_display, m_surface);
        }
        eglTerminate(m_display);
    }
    m_display = EGL_NO_DISPLAY;
    m_context = EGL_NO_CONTEXT;
    m_surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
int32_t lol::AndroidAppData::HandleInput(AInputEvent* event)
{
    switch (AInputEvent_getType(event))
    {
    case AINPUT_EVENT_TYPE_MOTION:
        //We need the max if we want coherent mouse speed between axis
        float max_screen_size = lol::max(m_wanted_resolution.x, m_wanted_resolution.y);
        /* FIXME: we flip the Y axis here, but is it the right place? */
        ivec2 pos(AMotionEvent_getX(event, 0),
                  AMotionEvent_getY(event, 0));
        pos = pos * m_wanted_resolution / Video::GetSize();
        pos.y = m_wanted_resolution.y - 1 - pos.y;
        m_mouse->SetCursor(0, vec2(pos) / vec2(m_wanted_resolution), pos);
        // Note: 100.0f is an arbitrary value that makes it feel about the same than an xbox controller joystick
        m_mouse->SetAxis(0, (pos.x - m_prev_pos.x) / max_screen_size * 100.f);
        // Unlike SDL, no need to negate Y axis
        m_mouse->SetAxis(1, (pos.y - m_prev_pos.y) / max_screen_size * -100.f);
        m_prev_pos = pos;
        switch (AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK)
        {
        case AMOTION_EVENT_ACTION_DOWN:
            m_mouse->SetKey(0, true);
            break;
        case AMOTION_EVENT_ACTION_UP:
            m_mouse->SetKey(0, false);
            break;
        }
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
void lol::AndroidAppData::HandleCommand(int32_t cmd)
{
    switch (cmd)
    {
        case APP_CMD_SAVE_STATE:
            /* The system has asked us to save our current state. Do so. */
            m_native_app->savedState = malloc(sizeof(SavedState));
            *((SavedState*)m_native_app->savedState) = m_state;
            m_native_app->savedStateSize = sizeof(SavedState);
            break;
        case APP_CMD_INIT_WINDOW:
            /* The window is being shown, get it ready. */
            if (m_native_app->window != nullptr)
            {
                CreateDisplay();
                DrawFrame();
            }
            break;
        case APP_CMD_TERM_WINDOW:
            /* The window is being hidden or closed, clean it up. */
            DestroyDisplay();
            break;
        case APP_CMD_GAINED_FOCUS:
            break;
        case APP_CMD_LOST_FOCUS:
            /* FIXME: stop animating */
            DrawFrame();
            break;
    }
}

/* FIXME: find a better way to pass this to the AndroidApp ctor. */
AndroidAppData *g_data;

void android_main(android_app* native_app)
{
    g_data = new AndroidAppData();
    g_data->m_native_app = native_app;

    /* Make sure glue isn't stripped */
    app_dummy();

    native_app->userData = g_data;
    native_app->onAppCmd = lol::AndroidAppData::StaticHandleCommand;
    native_app->onInputEvent = lol::AndroidAppData::StaticHandleInput;

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
    m_data->m_wanted_resolution = res;
    m_data->m_mouse = InputDeviceInternal::CreateStandardMouse();
}

void lol::AndroidApp::ShowPointer(bool show)
{
}

lol::AndroidApp::~AndroidApp()
{
    m_data->DestroyDisplay();
    /* FIXME: handle m_data->m_mouse */
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

    m_data->DrawFrame();
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

