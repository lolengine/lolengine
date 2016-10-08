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

#pragma once

//
// The sampler class
// -----------------
// The sampler is a static class that manages samples.
//

#include <stdint.h>

namespace lol
{

class sampler
{
public:
    static int load_sample(char const *path);
    static void unload_sample(int id);

    static void play_sample(int id);
    static void loop_sample(int id);
    static void stop_sample(int id);

private:
    sampler() {}
};

} /* namespace lol */

