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
#include <ppapi/cpp/input_event.h>

#include "core.h"

#include "platform/nacl/nacl-instance.h"
#include "platform/nacl/opengl_context.h"

/* One of these wrappers will be overridden by the user's version */
void lol_nacl_main(void) __attribute__((weak));
void lol_nacl_main(void) {}
void lol_nacl_main(int argc, char **argv) __attribute__((weak));
void lol_nacl_main(int argc, char **argv) {}
void lol_nacl_main(int argc, char **argv, char **envp) __attribute__((weak));
void lol_nacl_main(int argc, char **argv, char **envp) {}

namespace lol
{

NaClInstance::NaClInstance(PP_Instance instance)
    : pp::Instance(instance),
      m_size(0, 0)
{
    RequestInputEvents(PP_INPUTEVENT_CLASS_MOUSE);
}

NaClInstance::~NaClInstance()
{
    // Destroy the cube view while GL context is current.
    m_opengl_ctx->MakeContextCurrent(this);
}

static double const DELTA_MS = 1000.0 / 60.0;

void NaClInstance::TickCallback(void* data, int32_t result)
{
    NaClInstance *instance = (NaClInstance *)data;
    instance->DrawSelf();

    /* FIXME: only set if if Ticker isn't finished */
    pp::Module::Get()->core()->CallOnMainThread(
            DELTA_MS, pp::CompletionCallback(&TickCallback, data), PP_OK);

    /* Propagate gamepad information */
    PP_GamepadsSampleData all_pads_data;
    instance->m_pad_interface->Sample(instance->pp_instance(), &all_pads_data);

    for (int i = 0; i < all_pads_data.length; i++)
    {
        PP_GamepadSampleData const& pad_data = all_pads_data.items[i];

        if (i >= instance->m_sticks.Count())
        {
            Stick *stick = Input::CreateStick();
            instance->m_sticks.Push(stick);
        }

        instance->m_sticks[i]->SetAxisCount(pad_data.axes_length);
        for (int j = 0; j < pad_data.axes_length; j++)
            instance->m_sticks[i]->SetAxis(j, pad_data.axes[j]);

        instance->m_sticks[i]->SetButtonCount(pad_data.buttons_length);
        for (int j = 0; j < pad_data.buttons_length; j++)
            instance->m_sticks[i]->SetButton(j, pad_data.buttons[j] > 0.5f);
    }
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
}

void NaClInstance::DidChangeView(const pp::Rect& position, const pp::Rect& clip)
{
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
    switch (event.GetType())
    {
    case PP_INPUTEVENT_TYPE_MOUSEDOWN:
        Input::SetMouseButton(pp::MouseInputEvent(event).GetButton());
        break;
    case PP_INPUTEVENT_TYPE_MOUSEUP:
        Input::UnsetMouseButton(pp::MouseInputEvent(event).GetButton());
        break;
    case PP_INPUTEVENT_TYPE_MOUSEMOVE:
        Input::SetMousePos(ivec2(pp::MouseInputEvent(event).GetPosition().x(), m_opengl_ctx->GetSize().height() - 1 - pp::MouseInputEvent(event).GetPosition().y()));
        break;
    default:
        break;
    }
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

}  // namespace lol

