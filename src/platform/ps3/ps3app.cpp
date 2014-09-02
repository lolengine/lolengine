//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if defined __CELLOS_LV2__
#   include <sys/ppu_thread.h> /* sys_ppu_thread_get_stack_information */
#   include <sys/spu_initialize.h>
#   include <sys/paths.h> /* SYS_HOST_ROOT */
#   include <cell/sysmodule.h>
#   include <PSGL/psgl.h>

#   include <cell/mstream.h> /* multistream */

#   include <cell/spurs/control.h> /* SPURS */
#   include <cell/spurs/task.h>
#   include <cell/spurs/event_flag.h>
#endif

#include "lolgl.h"
#include "ps3app.h"
#include "ps3input.h"

namespace lol
{

/*
 * PS3 App implementation class
 */

/* FIXME: this shouldn't be a global */
static unsigned int port_num;

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

    static void MultiStreamThread(uint64_t param)
    {
        Timer t;
        cellAudioPortStart(port_num);
        /* FIXME: quit gracefully if needed */
        while (true)
        {
            if (!cellMSSystemSignalSPU())
            {
               t.Get();
               t.Wait(1.f / 60.f / 32);
            }

            cellMSSystemGenerateCallbacks();
        }
        cellAudioPortStop(port_num);
        sys_ppu_thread_exit(0);
    }

    CellSpurs m_spurs __attribute__((aligned (128)));
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

    cellSysutilRegisterCallback(0, Ps3AppData::SysCallBack, nullptr);

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

    /* Create audio device */
    cellSysmoduleLoadModule(CELL_SYSMODULE_IO);
    cellSysmoduleLoadModule(CELL_SYSMODULE_AUDIO);
    cellSysmoduleLoadModule(CELL_SYSMODULE_RESC);
    cellSysmoduleLoadModule(CELL_SYSMODULE_SPURS);

    int ret = cellMSSystemConfigureSysUtilEx(CELL_MS_AUDIOMODESELECT_SUPPORTSLPCM
                                           | CELL_MS_AUDIOMODESELECT_SUPPORTSDOLBY
                                           | CELL_MS_AUDIOMODESELECT_SUPPORTSDTS
                                           | CELL_MS_AUDIOMODESELECT_PREFERDOLBY);
    int num_chans = ret & 0xf;
    int has_dolby = (ret & 0x10) >> 4;
    int has_dts = (ret & 0x20) >> 5;

    Log::Debug("audio channels %d, dolby %d, DTS %d\n",
               num_chans, has_dolby, has_dts);
    ret = cellAudioInit();

    CellAudioPortParam ap;
    memset(&ap, 0, sizeof(ap));
    ap.nChannel = CELL_AUDIO_PORT_8CH;
    ap.nBlock = CELL_AUDIO_BLOCK_8;
    ret = cellAudioPortOpen(&ap, &port_num);
    Log::Debug("audio port %d\n", port_num);

    CellAudioPortConfig pc;
    ret = cellAudioGetPortConfig(port_num, &pc);

    cellMSSystemConfigureLibAudio(&ap, &pc);

    CellMSSystemConfig cfg;
    cfg.channelCount = 400; /* Maximum number of streams */
    cfg.subCount = 31; /* ? */
    cfg.dspPageCount = 5;
    cfg.flags = CELL_MS_NOFLAGS;

    uint8_t const prios[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };
    int mem_needed = cellMSSystemGetNeededMemorySize(&cfg);
    void *ms_mem = memalign(128, mem_needed);
    sys_ppu_thread_t tid;
    sys_ppu_thread_get_id(&tid);
    int tprio;
    sys_ppu_thread_get_priority(tid, &tprio);
    cellSpursInitialize(&data->m_spurs, 1, 250 /* thread group priority */,
                        tprio - 1, false);
    cellMSSystemInitSPURS(ms_mem, &cfg, &data->m_spurs, &prios[0]);

    float const bus_vols[64] =
    {
        1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f,
    };

    sys_ppu_thread_t thread;
    ret = sys_ppu_thread_create(&thread, Ps3AppData::MultiStreamThread,
                                nullptr, 0 /* Server prio */,
                                0x4000 /* 16 KiB stack */,
                                SYS_PPU_THREAD_CREATE_JOINABLE,
                                "Audio Thread");

    cellMSCoreSetVolume64(CELL_MS_SUBBUS_1, CELL_MS_DRY, bus_vols);
    cellMSCoreSetVolume64(CELL_MS_MASTER_BUS, CELL_MS_DRY, bus_vols);

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

    /* Unload audio modules */
    cellSysmoduleUnloadModule(CELL_SYSMODULE_IO);
    cellSysmoduleUnloadModule(CELL_SYSMODULE_AUDIO);
    cellSysmoduleUnloadModule(CELL_SYSMODULE_RESC);
    cellSysmoduleUnloadModule(CELL_SYSMODULE_SPURS);
#endif
    delete data;
}

} /* namespace lol */

