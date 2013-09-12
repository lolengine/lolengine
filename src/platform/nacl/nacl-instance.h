// Copyright (c) 2011 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXAMPLES_TUMBLER_TUMBLER_H_
#define EXAMPLES_TUMBLER_TUMBLER_H_

#include <ppapi/cpp/instance.h>
#include <ppapi/c/ppb_gamepad.h>

#include "platform/nacl/opengl_context.h"
#include "platform/nacl/opengl_context_ptrs.h"

#include "input/input.h"

namespace lol {

class NaClInstance : public pp::Instance
{
public:
    explicit NaClInstance(PP_Instance instance);

    // The dtor makes the 3D context current before deleting the cube view, then
    // destroys the 3D context both in the module and in the browser.
    virtual ~NaClInstance();

    // Called by the browser when the NaCl module is loaded and all ready to go.
    virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);

    // Called whenever the in-browser window changes size.
    virtual void DidChangeView(const pp::Rect& position, const pp::Rect& clip);

    // Called by the browser to handle the postMessage() call in Javascript.
    virtual void HandleMessage(const pp::Var& message);

    // Bind and publish the module's methods to JavaScript.
    //void InitializeMethods(ScriptingBridge* bridge);

    // Called to draw the contents of the module's browser area.
    virtual bool HandleInputEvent(const pp::InputEvent& event);

    void DrawSelf();

    /* Communication with the application object */
    static void MainSignal();

private:
    SharedOpenGLContext m_opengl_ctx;

    ivec2 m_size;

    static void TickCallback(void* data, int32_t result);
    static void * MainRun(void *data);

    /* Gamepad support */
    PPB_Gamepad const *m_pad_interface;
    //12/09/2013 : Should use new system. Array<Stick *> m_sticks;
    Array<InputDevice*> m_sticks;

    /* Communication with the application object */
    struct Args
    {
        Args(int argc, char **argv, char **env)
          : m_argc(argc), m_argv(argv), m_env(env) {}
        Args() {}

        int m_argc;
        char **m_argv;
        char **m_env;
    };
    static Mutex main_mutex;
    static Queue<Args *, 1> main_queue;

    Thread *m_main_thread;
};

}  // namespace lol

#endif  // EXAMPLES_TUMBLER_TUMBLER_H_
