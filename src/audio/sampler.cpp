//
//  Lol Engine
//
//  Copyright © 2010—2016 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

namespace lol
{

/*
 * sampler implementation class
 */

static class sampler_data
{
    friend class sampler;

public:
    Dict samples;
}
samplerdata;

static sampler_data * const data = &samplerdata;

/*
 * Public sampler class
 */

int sampler::load_sample(char const *path)
{
    int id = data->samples.MakeSlot(path);

    if (!data->samples.GetEntity(id))
    {
        sample *s = new sample(path);
        data->samples.SetEntity(id, s);
    }

    return id + 1; /* ID 0 is for the empty sample */
}

void sampler::unload_sample(int id)
{
    if (id > 0)
        data->samples.RemoveSlot(id - 1); /* ID 0 is for the empty sample */
}

void sampler::play_sample(int id)
{
    if (id > 0)
    {
        sample *s = (sample *)data->samples.GetEntity(id - 1);
        s->play();
    }
}

void sampler::loop_sample(int id)
{
    if (id > 0)
    {
        sample *s = (sample *)data->samples.GetEntity(id - 1);
        s->loop();
    }
}

void sampler::stop_sample(int id)
{
    if (id > 0)
    {
        sample *s = (sample *)data->samples.GetEntity(id - 1);
        s->stop();
    }
}

} /* namespace lol */

