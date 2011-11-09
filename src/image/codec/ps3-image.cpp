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

#include <cmath>

#include <cell/sysmodule.h>
#include <cell/codec/pngdec.h>

#include "core.h"
#include "../../image/image-private.h"

using namespace std;

namespace lol
{

/*
 * Image implementation class
 */

DECLARE_IMAGE_LOADER(Ps3ImageData, 100)
{
public:
    virtual bool Open(char const *);
    virtual bool Close();

    virtual void *GetData() const;

private:
    static void* Malloc(uint32_t size, void* data) { return malloc(size); };
    static int32_t Free(void* ptr, void* data) { free(ptr); return 0; };
    uint8_t *pixels;
};

/*
 * Public Image class
 */

bool Ps3ImageData::Open(char const *path)
{
    int32_t err;

    /* Initialise decoding library */
    CellPngDecMainHandle hmain;

    err = cellSysmoduleLoadModule(CELL_SYSMODULE_FS);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not open Fs sysmodule\n");
#endif
        return false;
    }

    err = cellSysmoduleLoadModule(CELL_SYSMODULE_PNGDEC);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not open PngDec sysmodule\n");
#endif
        return false;
    }

    CellPngDecThreadInParam in_param;
    in_param.spuThreadEnable = CELL_PNGDEC_SPU_THREAD_ENABLE;
    in_param.ppuThreadPriority = 1000;
    in_param.spuThreadPriority = 200;
    in_param.cbCtrlMallocFunc = Ps3ImageData::Malloc;
    in_param.cbCtrlMallocArg = NULL;
    in_param.cbCtrlFreeFunc = Ps3ImageData::Free;
    in_param.cbCtrlFreeArg = NULL;
    CellPngDecThreadOutParam out_param;
    err = cellPngDecCreate(&hmain, &in_param, &out_param);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not create PngDec library\n");
#endif
        return false;
    }

    /* Create decoder */
    CellPngDecSubHandle hsub;

    char file[1024];
    sprintf(file, "/app_home/c:/Users/s.hocevar/le/%s", path);

    CellPngDecSrc dec_src;
    dec_src.srcSelect = CELL_PNGDEC_FILE;
    dec_src.fileName = file;
    dec_src.fileOffset = 0;
    dec_src.fileSize = 0;
    dec_src.streamPtr = NULL;
    dec_src.streamSize = 0;
    dec_src.spuThreadEnable  = CELL_PNGDEC_SPU_THREAD_ENABLE;
    CellPngDecOpnInfo open_info;
    err = cellPngDecOpen(hmain, &hsub, &dec_src, &open_info);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not open %s for decoding\n", file);
#endif
        return false;
    }

    CellPngDecInfo info;
    err = cellPngDecReadHeader(hmain, hsub, &info);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not read image header\n");
#endif
        return false;
    }

    CellPngDecInParam in_dec_param;
    in_dec_param.commandPtr = NULL;
    in_dec_param.outputMode = CELL_PNGDEC_TOP_TO_BOTTOM;
    in_dec_param.outputColorSpace = CELL_PNGDEC_RGBA;
    in_dec_param.outputBitDepth = 8;
    in_dec_param.outputPackFlag = CELL_PNGDEC_1BYTE_PER_1PIXEL;
    in_dec_param.outputAlphaSelect = CELL_PNGDEC_STREAM_ALPHA;
    in_dec_param.outputColorAlpha = 0xff;
    CellPngDecOutParam out_dec_param;
    err = cellPngDecSetParameter(hmain, hsub, &in_dec_param, &out_dec_param);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not configure PngDec decoder\n");
#endif
        return false;
    }

    /* Decode image */
    size = ivec2(info.imageWidth, info.imageHeight);
    format = Image::FORMAT_RGBA;
    pixels = (uint8_t *)malloc(info.imageWidth * 4 * info.imageHeight);
    CellPngDecDataCtrlParam data_ctrl_param;
    data_ctrl_param.outputBytesPerLine = info.imageWidth * 4;
    CellPngDecDataOutInfo data_out_info;
    err = cellPngDecDecodeData(hmain, hsub, pixels,
                               &data_ctrl_param, &data_out_info);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not run PngDec decoder\n");
#endif
        return false;
    }

    /* Close decoder */
    err = cellPngDecClose(hmain, hsub);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not close PngDec decoder\n");
#endif
        return false;
    }

    /* Deinitialise library */
    err = cellPngDecDestroy(hmain);
    if (err != CELL_OK)
    {
#if !LOL_RELEASE
        Log::Error("could not destroy PngDec decoder\n");
#endif
        return false;
    }
    err = cellSysmoduleUnloadModule(CELL_SYSMODULE_PNGDEC);
    err = cellSysmoduleUnloadModule(CELL_SYSMODULE_FS);

    return true;
}

bool Ps3ImageData::Close()
{
    free(pixels);

    return true;
}

void * Ps3ImageData::GetData() const
{
    return pixels;
}

} /* namespace lol */

#endif /* defined __CELLOS_LV2__ */

