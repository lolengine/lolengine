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

#include <cstdlib>
#include <cmath>

#include <SDL.h>

#include "core.h"

/*
 * Sample implementation class
 */

class SampleData
{
    friend class Sample;

private:
    char *name;
};

/*
 * Public Sample class
 */

Sample::Sample(char const *path)
{
    data = new SampleData();
    data->name = strdup(path);
}

Sample::~Sample()
{
    free(data->name);
    delete data;
}

void Sample::TickGame(float deltams)
{
    Entity::TickGame(deltams);
}

char const *Sample::GetName()
{
    return data->name;
}

void Sample::Play()
{
}

