//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

namespace lol
{

/*
 * Sampler implementation class
 */

static class SamplerData
{
    friend class Sampler;

public:
    Dict samples;
}
samplerdata;

static SamplerData * const data = &samplerdata;

/*
 * Public Sampler class
 */

int Sampler::Register(char const *path)
{
    int id = data->samples.MakeSlot(path);

    if (!data->samples.GetEntity(id))
    {
        Sample *sample = new Sample(path);
        data->samples.SetEntity(id, sample);
    }

    return id + 1; /* ID 0 is for the empty sample */
}

void Sampler::Deregister(int id)
{
    data->samples.RemoveSlot(id - 1); /* ID 0 is for the empty sample */
}

void Sampler::PlaySample(int id)
{
    Sample *sample = (Sample *)data->samples.GetEntity(id - 1);
    sample->Play();
}

void Sampler::StopSample(int id)
{
    Sample *sample = (Sample *)data->samples.GetEntity(id - 1);
    sample->Stop();
}

} /* namespace lol */

