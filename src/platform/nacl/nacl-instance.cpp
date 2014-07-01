// Copyright (c) 2011 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>

#include <ppapi/cpp/rect.h>
#include <ppapi/cpp/size.h>
#include <ppapi/cpp/var.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/completion_callback.h>

#include <lol/main.h>

#include "platform/nacl/nacl-instance.h"
#include "platform/nacl/opengl_context.h"

/* One of these wrappers will be overridden by the user's version */
void lol_nacl_main(void) __attribute__((weak));
void lol_nacl_main(void) {}
void lol_nacl_main(int argc, char **argv) __attribute__((weak));
void lol_nacl_main(int argc, char **argv) { UNUSED(argc, argv); }
void lol_nacl_main(int argc, char **argv, char **envp) __attribute__((weak));
void lol_nacl_main(int argc, char **argv, char **envp) { UNUSED(argc, argv, envp); }

namespace lol
{

/*
 * NACL Input implementation class
 * This is a ripoff of the SDL one
 */

NaClInstance::NaClInstance(PP_Instance instance)
    : pp::Instance(instance),
      m_size(0, 0)
{
    RequestInputEvents(PP_INPUTEVENT_CLASS_MOUSE | PP_INPUTEVENT_CLASS_WHEEL);
    RequestFilteringInputEvents(PP_INPUTEVENT_CLASS_KEYBOARD);

    m_input_data = new NaClInputData();
    m_input_data->m_keyboard = InputDeviceInternal::CreateStandardKeyboard();
    m_input_data->m_mouse = InputDeviceInternal::CreateStandardMouse();
}

NaClInstance::~NaClInstance()
{
    // Destroy the cube view while GL context is current.
    m_opengl_ctx->MakeContextCurrent(this);
    delete m_input_data;
}

static double const DELTA_MS = 1000.0 / 60.0;

void NaClInstance::TickCallback(void* data, int32_t result)
{
    UNUSED(result);
    NaClInstance *instance = (NaClInstance *)data;
    instance->DrawSelf();

    /* FIXME: only set if if Ticker isn't finished */
    pp::Module::Get()->core()->CallOnMainThread(
            DELTA_MS, pp::CompletionCallback(&TickCallback, data), PP_OK);

    //Tick input : TODO: DELTA_MS is not exactly kasher ?
    instance->m_input_data->Tick(DELTA_MS);
}

Mutex NaClInstance::main_mutex;
Queue<NaClInstance::Args *, 1> NaClInstance::main_queue;

bool NaClInstance::Init(uint32_t argc,
                        const char* /* argn */[],
                        const char* argv[])
{
    /* Ensure only one NaClInstance does Init() at the same time. */
    main_mutex.Lock();
    char *env[] = { nullptr };
    Args arglist(argc, const_cast<char **>(argv), const_cast<char **>(env));
    main_queue.Push(&arglist);
    m_main_thread = new Thread(MainRun, nullptr);
    /* Push so that only MainSignal() can unblock us */
    main_queue.Push(nullptr);
    main_queue.Push(nullptr);
    main_mutex.Unlock();

    // My timer callback
    pp::Module::Get()->core()->CallOnMainThread(
            DELTA_MS, pp::CompletionCallback(&TickCallback, this), PP_OK);

    /* The gamepad interface */
    m_pad_interface = static_cast<PPB_Gamepad const *>(
            pp::Module::Get()->GetBrowserInterface(PPB_GAMEPAD_INTERFACE));

    return true;
}

void * NaClInstance::MainRun(void *data)
{
    UNUSED(data);
    Args *arglist = main_queue.Pop();

    /* Call the user's main() function. One of these will work. */
    lol_nacl_main();
    lol_nacl_main(arglist->m_argc, arglist->m_argv);
    lol_nacl_main(arglist->m_argc, arglist->m_argv, arglist->m_env);

    return nullptr;
}

void NaClInstance::MainSignal()
{
    /* FIXME: find something more elegant. */
    main_queue.Pop();
    main_queue.Pop();
}

void NaClInstance::HandleMessage(const pp::Var& message)
{
    if (!message.is_string())
        return;

    /* FIXME: do some shit here */
    MessageService::Send(MessageBucket::AppIn, message.AsString().c_str());
}

void NaClInstance::DidChangeView(const pp::Rect& position, const pp::Rect& clip)
{
    UNUSED(clip);
    if (position.size().width() == m_size.x &&
        position.size().height() == m_size.y)
        return;  // Size didn't change, no need to update anything.

    m_size = ivec2(position.size().width(), position.size().height());

    if (m_opengl_ctx == nullptr)
        m_opengl_ctx.reset(new OpenGLContext(this));
    m_opengl_ctx->InvalidateContext(this);
    m_opengl_ctx->ResizeContext(position.size());
    if (!m_opengl_ctx->MakeContextCurrent(this))
        return;

    Video::Setup(m_size);
    DrawSelf();
}

bool NaClInstance::HandleInputEvent(const pp::InputEvent& event)
{
    m_input_data->m_input_events << event;
    return true;
}

void NaClInstance::DrawSelf()
{
    if (m_opengl_ctx == nullptr)
        return;

    m_opengl_ctx->MakeContextCurrent(this);
    Ticker::TickDraw();
    m_opengl_ctx->FlushContext();
}

void NaClInputData::Tick(float seconds)
{
    UNUSED(seconds);
    if (!IsViewportSizeValid())
        InitViewportSize();

    //Init cursor position, if mouse didn't move.
    ivec2 mousepos = m_mouse->GetCursorPixelPos(0);
    vec2 mousepos_prev = vec2(mousepos);
    /* Handle keyboard and WM events */
    for (int i = 0; i < m_input_events.Count(); ++i)
    {
        pp::InputEvent &e = m_input_events[i];
        switch (e.GetType())
        {
            case PP_INPUTEVENT_TYPE_MOUSEDOWN:
            case PP_INPUTEVENT_TYPE_MOUSEUP:
            {
                pp::MouseInputEvent em = pp::MouseInputEvent(e);
                //Show probably do a switch.
                if (em.GetButton() != PP_INPUTEVENT_MOUSEBUTTON_NONE)
                    m_mouse->SetKey(em.GetButton(), em.GetType() == PP_INPUTEVENT_TYPE_MOUSEDOWN);
                break;
            }
            case PP_INPUTEVENT_TYPE_MOUSELEAVE:
            case PP_INPUTEVENT_TYPE_MOUSEENTER:
            {
                /* TODO: "InScreen" idx hardcoded, not nice */
                pp::MouseInputEvent em = pp::MouseInputEvent(e);
                m_mouse->SetKey(3, em.GetType() == PP_INPUTEVENT_TYPE_MOUSEENTER);
                break;
            }
            case PP_INPUTEVENT_TYPE_MOUSEMOVE:
            {
                pp::MouseInputEvent em = pp::MouseInputEvent(e);
                mousepos = ivec2(em.GetPosition().x(), em.GetPosition().y());
                break;
            }
            case PP_INPUTEVENT_TYPE_WHEEL:
            {
                /* TODO: MOUSE WHEEL NOT IMPLEMENTED IN LOL */
                break;
            }
            // Use ?
            //case PP_INPUTEVENT_TYPE_RAWKEYDOWN:
            //case PP_INPUTEVENT_TYPE_CHAR:
            case PP_INPUTEVENT_TYPE_KEYDOWN:
            case PP_INPUTEVENT_TYPE_KEYUP:
            {
                pp::KeyboardInputEvent ek = pp::KeyboardInputEvent(e);
                m_keyboard->SetKey(ek.GetKeyCode(), ek.GetType() == PP_INPUTEVENT_TYPE_KEYUP);
                break;
            }
            case PP_INPUTEVENT_TYPE_UNDEFINED:
            default:
            {
                break;
            }
        }
    }
    m_input_events.Empty();

    /* Handle mouse input */
    if (IsViewportSizeValid())
    {
        if (mousepos.x >= 0 && mousepos.x < m_app.x && mousepos.y >= 0 && mousepos.y < m_app.y)
        {
            //We need the max if we want coherent mouse speed between axis
            float max_screen_size = lol::max(m_screen.x, m_screen.y);
            vec2 vmousepos = vec2(mousepos);
            m_mouse->SetCursor(0, vmousepos / m_app, mousepos);
            // Note: 100.0f is an arbitrary value that makes it feel about the same than an xbox controller joystick
            m_mouse->SetAxis(0, (vmousepos.x - mousepos_prev.x) * 100.0f / max_screen_size);
            // Unlike SDL, no need to negate Y axis.
            m_mouse->SetAxis(1, (vmousepos.y - mousepos_prev.y) * 100.0f / max_screen_size);
        }

        if (m_mousecapture)
        {
            /*
            mousepos = ivec2(m_app * .5f);
            NaClInputData::SetMousePos(mousepos);
            */
        }
    }
}

//----
void NaClInputData::InitViewportSize()
{
    if (g_scene)
    {
        m_app = vec2(Video::GetSize());
        //Dunno if its the good idea.
        m_screen = vec2(Video::GetSize());
    }
    else
    {
        m_app = vec2(-1.f);
        m_screen = vec2(-1.f);
    }
}

void NaClInputData::SetMousePos(ivec2 position)
{
    UNUSED(position);
    //? How to do that ?
    //SDL_WarpMouse((uint16_t)position.x, (uint16_t)position.y);
}

}  // namespace lol

