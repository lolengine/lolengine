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

#if defined __CELLOS_LV2__
#   include <sys/ppu_thread.h> /* sys_ppu_thread_get_stack_information */
#   include <sys/spu_initialize.h>
#   include <sys/paths.h> /* SYS_HOST_ROOT */
#   include <cell/sysmodule.h>
#   include <PSGL/psgl.h>
#endif

#include "core.h"
#include "lolgl.h"
#include "ps3app.h"
#include "ps3input.h"

namespace lol
{

/*
 * PS3 App implementation class
 */

class Ps3AppData
{
    friend class Ps3App;

private:
#if defined __CELLOS_LV2__
    static void SysCallBack(uint64_t status, uint64_t param, void *data)
    {
        if (status == CELL_SYSUTIL_REQUEST_EXITGAME)
            Ticker::Shutdown();
    }
#endif
};

/*
 * Public Ps3App class
 */

Ps3App::Ps3App(char const *title, ivec2 res, float fps) :
    data(new Ps3AppData())
{
#if defined __CELLOS_LV2__
    sys_spu_initialize(6, 1);

    /* FIXME: we should check for CELL_SYSMODULE_ERROR_UNKNOWN and others,
     * but what could we do anyway? */
    cellSysmoduleLoadModule(CELL_SYSMODULE_GCM_SYS);
    cellSysmoduleLoadModule(CELL_SYSMODULE_FS);
    cellSysmoduleLoadModule(CELL_SYSMODULE_USBD);
    cellSysmoduleLoadModule(CELL_SYSMODULE_IO);

    cellSysutilRegisterCallback(0, Ps3AppData::SysCallBack, NULL);

    PSGLinitOptions psglio =
    {
        enable: PSGL_INIT_MAX_SPUS | PSGL_INIT_INITIALIZE_SPUS
                 | PSGL_INIT_HOST_MEMORY_SIZE,
        maxSPUs: 1,
        initializeSPUs: false,
        persistentMemorySize: 0,
        transientMemorySize: 0,
        errorConsole: 0,
        fifoSize: 0,
        hostMemorySize: 128 * 1024 * 1024,
    };

    psglInit(&psglio);

#if 0
    sys_ppu_thread_stack_t stack;
    sys_ppu_thread_get_stack_information(&stack);
    printf("stack starts at %p, ends at %p\n", stack.pst_addr,
           (uint8_t *)stack.pst_addr + stack.pst_size);
#endif

    PSGLdevice* psgl = psglCreateDeviceAuto(GL_ARGB_SCE, GL_DEPTH_COMPONENT24,
                                       GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE);
    GLuint w, h;
    psglGetDeviceDimensions(psgl, &w, &h);
    res = ivec2(w, h);

    PSGLcontext *ctx = psglCreateContext();
    psglMakeCurrent(ctx, psgl);
    /* TODO: load our shaders when we actually ship them */
    //psglLoadShaderLibrary("/shaders.bin");
    psglResetCurrentContext();

    /* Initialise everything */
    Ticker::Setup(fps);
    Video::Setup(res);
    Audio::Setup(2);

    /* Autoreleased objects */
    new Ps3Input();
#endif
}

void Ps3App::ShowPointer(bool show)
{
    ;
}

void Ps3App::Run()
{
    while (!Ticker::Finished())
    {
        /* Tick the game */
        Ticker::TickGame();

        /* Tick the renderer, show the frame and clamp to desired framerate. */
        Ticker::TickDraw();

#if defined __CELLOS_LV2__
        psglSwap();

        /* Check if exit callback was called */
        cellSysutilCheckCallback();
#endif

        Ticker::ClampFps();
    }
}

Ps3App::~Ps3App()
{
#if defined __CELLOS_LV2__
    glFinish();
#endif
    delete data;
}

} /* namespace lol */

