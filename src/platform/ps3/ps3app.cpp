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

    PSGLinitOptions psglio;
    psglio.enable = PSGL_INIT_MAX_SPUS
                  | PSGL_INIT_INITIALIZE_SPUS
                  | PSGL_INIT_HOST_MEMORY_SIZE;
    psglio.maxSPUs = 1;
    psglio.initializeSPUs = false;
    psglio.hostMemorySize = 128 * 1024 * 1024;

    psglInit(&psglio);

#if 0
    sys_ppu_thread_stack_t stack;
    sys_ppu_thread_get_stack_information(&stack);
    printf("stack starts at %p, ends at %p\n", stack.pst_addr,
           (uint8_t *)stack.pst_addr + stack.pst_size);
#endif

    PSGLdeviceParameters psgldp;
    psgldp.enable = PSGL_DEVICE_PARAMETERS_COLOR_FORMAT
                  | PSGL_DEVICE_PARAMETERS_DEPTH_FORMAT
                  | PSGL_DEVICE_PARAMETERS_MULTISAMPLING_MODE
                  | PSGL_DEVICE_PARAMETERS_WIDTH_HEIGHT;
    psgldp.colorFormat = GL_ARGB_SCE; /* can also be GL_RGBA16F_ARB */
    psgldp.depthFormat = GL_DEPTH_COMPONENT16; /* can also be 24-bit */
    psgldp.multisamplingMode = GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE;
    psgldp.width = 720;
    psgldp.height = 480;

    /* Find closest valid resolution */
    ivec2 const valid_resolutions[8] =
    {
        ivec2( 720,  480),
        ivec2( 720,  576),
        ivec2(1280,  720),
        ivec2(1920, 1080),
        ivec2( 960, 1080),
        ivec2(1280, 1080),
        ivec2(1440, 1080),
        ivec2(1600, 1080),
    };

    for (int i = 0; i < 8; ++i)
    {
        ivec2 cur(psgldp.width, psgldp.height);
        if (sqlength(valid_resolutions[i] - res) < sqlength(cur - res))
        {
            psgldp.width = valid_resolutions[i].x;
            psgldp.height = valid_resolutions[i].y;
        }
    }

    /* Create graphics device */
    PSGLdevice* psgl = psglCreateDeviceExtended(&psgldp);
    if (!psgl)
        Log::Error("could not create PSGL device; expect imminent crash\n");

    GLuint w, h;
    psglGetDeviceDimensions(psgl, &w, &h);
    ivec2 newres(w, h);
    Log::Debug("resolution asked %d×%d, closest valid %d×%d, final %d×%d\n",
               res.x, res.y, psgldp.width, psgldp.height, newres.x, newres.y);

    PSGLcontext *ctx = psglCreateContext();
    psglMakeCurrent(ctx, psgl);
    /* TODO: load our shaders when we actually ship them */
    //psglLoadShaderLibrary("/shaders.bin");
    psglResetCurrentContext();

    /* Initialise everything */
    Ticker::Setup(fps);
    Video::Setup(newres);
    Audio::Setup(2);

    /* Autoreleased objects */
    new Ps3Input();
#endif
}

void Ps3App::ShowPointer(bool show)
{
    ;
}

void Ps3App::Tick()
{
    /* Tick the renderer, show the frame and clamp to desired framerate. */
    Ticker::TickDraw();

#if defined __CELLOS_LV2__
    psglSwap();

    /* Check if exit callback was called */
    cellSysutilCheckCallback();
#endif
}

Ps3App::~Ps3App()
{
#if defined __CELLOS_LV2__
    glFinish();
#endif
    delete data;
}

} /* namespace lol */

