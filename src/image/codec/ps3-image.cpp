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

#include <sys/paths.h>
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

class Ps3ImageCodec : public ImageCodec
{
public:
    virtual bool Load(Image *image, char const *path);
    virtual bool Save(Image *image, char const *path);
    virtual bool Close();

    virtual uint8_t *GetData() const;

private:
    static void* Malloc(uint32_t size, void* data) { return malloc(size); };
    static int32_t Free(void* ptr, void* data) { free(ptr); return 0; };
};

DECLARE_IMAGE_CODEC(Ps3ImageCodec, 100)

/*
 * Public Image class
 */

bool Ps3ImageCodec::Load(Image *image, char const *path)
{
    int32_t err;

    /* Initialise decoding library */
    CellPngDecMainHandle hmain;

    err = cellSysmoduleLoadModule(CELL_SYSMODULE_FS);
    if (err != CELL_OK)
    {
        Log::Error("could not open Fs sysmodule (0x%08x)\n", err);
        return false;
    }

    err = cellSysmoduleLoadModule(CELL_SYSMODULE_PNGDEC);
    if (err != CELL_OK)
    {
        Log::Error("could not open PngDec sysmodule (0x%08x)\n", err);
        return false;
    }

    CellPngDecThreadInParam in_param;
    in_param.spuThreadEnable = CELL_PNGDEC_SPU_THREAD_ENABLE;
    in_param.ppuThreadPriority = 1000;
    in_param.spuThreadPriority = 200;
    in_param.cbCtrlMallocFunc = Ps3ImageCodec::Malloc;
    in_param.cbCtrlMallocArg = nullptr;
    in_param.cbCtrlFreeFunc = Ps3ImageCodec::Free;
    in_param.cbCtrlFreeArg = nullptr;
    CellPngDecThreadOutParam out_param;
    err = cellPngDecCreate(&hmain, &in_param, &out_param);
    if (err != CELL_OK)
    {
        Log::Error("could not create PngDec library (0x%08x)\n", err);
        return false;
    }

    /* Create decoder */
    CellPngDecSrc dec_src;
    dec_src.srcSelect = CELL_PNGDEC_FILE;
    dec_src.fileOffset = 0;
    dec_src.fileSize = 0;
    dec_src.streamPtr = nullptr;
    dec_src.streamSize = 0;
    dec_src.spuThreadEnable  = CELL_PNGDEC_SPU_THREAD_ENABLE;
    CellPngDecSubHandle hsub;
    CellPngDecOpnInfo open_info;

    Array<String> pathlist = System::GetPathList(path);
    for (int i = 0; i < pathlist.Count(); ++i)
    {
        String name = String(SYS_APP_HOME) + '/' + pathlist[i];
        dec_src.fileName = name.C();
        err = cellPngDecOpen(hmain, &hsub, &dec_src, &open_info);
        if (err == CELL_OK)
            break;
        cellPngDecClose(hmain, hsub);
    }

    if (err != CELL_OK)
    {
        Log::Error("could not open %s for decoding (0x%08x)\n", path, err);
        return false;
    }

    CellPngDecInfo info;
    err = cellPngDecReadHeader(hmain, hsub, &info);
    if (err != CELL_OK)
    {
        Log::Error("could not read image header in %s (0x%08x)\n", path, err);
        return false;
    }

    CellPngDecInParam in_dec_param;
    in_dec_param.commandPtr = nullptr;
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
        Log::Error("could not configure PngDec decoder (0x%08x)\n", err);
        return false;
    }

    /* Decode image */
    m_size = ivec2(info.imageWidth, info.imageHeight);
    m_format = PixelFormat::RGBA_8;
    pixels = (uint8_t *)malloc(info.imageWidth * 4 * info.imageHeight);
    CellPngDecDataCtrlParam data_ctrl_param;
    data_ctrl_param.outputBytesPerLine = info.imageWidth * 4;
    CellPngDecDataOutInfo data_out_info;
    err = cellPngDecDecodeData(hmain, hsub, pixels,
                               &data_ctrl_param, &data_out_info);
    if (err != CELL_OK)
    {
        Log::Error("could not run PngDec decoder on %s (0x%08x)\n", path, err);
        return false;
    }

    /* Close decoder */
    err = cellPngDecClose(hmain, hsub);
    if (err != CELL_OK)
    {
        Log::Error("could not close PngDec decoder (0x%08x)\n", err);
        return false;
    }

    /* Deinitialise library */
    err = cellPngDecDestroy(hmain);
    if (err != CELL_OK)
    {
        Log::Error("could not destroy PngDec decoder (0x%08x)\n", err);
        return false;
    }
    err = cellSysmoduleUnloadModule(CELL_SYSMODULE_PNGDEC);
    err = cellSysmoduleUnloadModule(CELL_SYSMODULE_FS);

    return true;
}

bool Ps3ImageCodec::Load(Image *image, char const *path)
{
    UNUSED(path);

    /* TODO: unimplemented */
    return true;
}

bool Ps3ImageCodec::Close()
{
    free(pixels);

    return true;
}

uint8_t * Ps3ImageCodec::GetData() const
{
    return pixels;
}

} /* namespace lol */

#endif /* defined __CELLOS_LV2__ */

