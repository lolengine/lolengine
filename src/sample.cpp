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

#include <cstdlib>
#include <cstdio>
#include <cstring>

#if __CELLOS_LV2__
#   include <cell/mstream.h> /* multistream */
#elif defined USE_SDL_MIXER
#   if defined HAVE_SDL_SDL_H
#      include <SDL/SDL.h>
#   else
#      include <SDL.h>
#   endif
#   if defined HAVE_SDL_SDL_MIXER_H
#      include <SDL/SDL_mixer.h>
#   else
#      include <SDL_mixer.h>
#   endif
#endif

#include "core.h"

using namespace std;

namespace lol
{

/*
 * Sample implementation class
 */

class SampleData
{
    friend class Sample;

private:
    String m_name;
#if __CELLOS_LV2__
    static void Callback(int stream, void *data, int type,
                         void *write_buf, int buf_size)
    {
        UNUSED(write_buf, buf_size);
        switch (type)
        {
        case CELL_MS_CALLBACK_MOREDATA:
            /* For streamed sound */
            break;
        case CELL_MS_CALLBACK_CLOSESTREAM:
        case CELL_MS_CALLBACK_FINISHSTREAM:
            /* Finished playing */
            break;
        default:
            /* FIXME: we can't reach here */
            break;
        }
    }

    String m_data;
    int m_channel;
#elif defined USE_SDL_MIXER
    Mix_Chunk *m_chunk;
    int m_channel;
#endif
};

/*
 * Public Sample class
 */

Sample::Sample(char const *path)
  : data(new SampleData())
{
    data->m_name = String("<sample> ") + path;

#if __CELLOS_LV2__
    Array<String> pathlist = System::GetPathList(path);
    File f;
    for (int i = 0; i < pathlist.Count(); ++i)
    {
        f.Open(pathlist[i], FileAccess::Read);
        if (f.IsValid())
        {
            data->m_data = f.ReadString();
            f.Close();
            break;
        }
    }
    if (data->m_data.Count() == 0)
    {
        Log::Error("could not load sample %s\n", path);
    }
#elif defined USE_SDL_MIXER
    Array<String> pathlist = System::GetPathList(path);
    for (int i = 0; i < pathlist.Count(); ++i)
    {
        data->m_chunk = Mix_LoadWAV(pathlist[i].C());
        if (data->m_chunk)
            break;
    }
    if (!data->m_chunk)
    {
        Log::Error("could not load sample %s\n", path);
    }
    data->m_channel = -1;
#endif
}

Sample::~Sample()
{
#if __CELLOS_LV2__
    /* Nothing to do */
#elif defined USE_SDL_MIXER
    if (data->m_chunk)
        Mix_FreeChunk(data->m_chunk);
#endif
    delete data;
}

void Sample::TickGame(float seconds)
{
    Entity::TickGame(seconds);
}

char const *Sample::GetName()
{
    return data->m_name.C();
}

void Sample::Play()
{
#if __CELLOS_LV2__
    if (data->m_data.Count() <= 64 + 256)
        return;

    data->m_channel = cellMSStreamOpen();

    CellMSInfo mi;
    mi.SubBusGroup = CELL_MS_MASTER_BUS;
    mi.FirstBuffer = &data->m_data[0] + 64;
    /* FIXME: hardcoded crap */
    mi.FirstBufferSize = data->m_data.Count() - 64 - 256;
    mi.SecondBuffer = nullptr;
    mi.SecondBufferSize = 0;

    mi.Pitch = 44100;
    mi.numChannels = 2;
    mi.flags = CELL_MS_STREAM_AUTOCLOSE;

    mi.initialOffset = 0;
    mi.inputType = CELL_MS_16BIT_LITTLE;
    cellMSStreamSetInfo(data->m_channel, &mi);

    cellMSStreamSetCallbackFunc(data->m_channel, SampleData::Callback);

    cellMSCoreSetVolume1(data->m_channel, CELL_MS_DRY,
                         CELL_MS_SPEAKER_FL, CELL_MS_CHANNEL_0, 1.0f);
    cellMSCoreSetVolume1(data->m_channel, CELL_MS_DRY,
                         CELL_MS_SPEAKER_FR, CELL_MS_CHANNEL_0, 1.0f);
    cellMSStreamPlay(data->m_channel);
#elif defined USE_SDL_MIXER
    if (data->m_chunk)
        data->m_channel = Mix_PlayChannel(-1, data->m_chunk, 0);
#endif
}

void Sample::Loop()
{
#if __CELLOS_LV2__
    /* FIXME: not implemented */
    Play();
#elif defined USE_SDL_MIXER
    if (data->m_chunk)
        data->m_channel = Mix_PlayChannel(-1, data->m_chunk, -1);
#endif
}

void Sample::Stop()
{
#if __CELLOS_LV2__
    if (data->m_channel >= 0)
        cellMSStreamClose(data->m_channel);
    data->m_channel = -1;
#elif defined USE_SDL_MIXER
    if (data->m_channel >= 0)
        Mix_HaltChannel(data->m_channel);
    data->m_channel = -1;
#endif
}

} /* namespace lol */

